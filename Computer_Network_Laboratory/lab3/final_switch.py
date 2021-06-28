from ryu.base import app_manager
from ryu.controller import ofp_event
from ryu.controller.handler import CONFIG_DISPATCHER, MAIN_DISPATCHER, DEAD_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3
from ryu.lib.packet import packet
from ryu.lib.packet import ethernet
from ryu.lib.packet import ether_types
from ryu.lib.packet import in_proto
from ryu.lib.packet import ipv4
from ryu.lib.packet import icmp
from ryu.lib.packet import tcp
from ryu.lib.packet import udp
from operator import attrgetter
from ryu.lib import hub
from ryu.lib import dpid as dpid_lib
from ryu.lib import stplib
import numpy as np

class SimpleSwitch13(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]
    _CONTEXTS = {'stplib': stplib.Stp}

    def __init__(self, *args, **kwargs):
        super(SimpleSwitch13, self).__init__(*args, **kwargs)
        self.mac_to_port = {}
        self.stp = kwargs['stplib']

        config = {dpid_lib.str_to_dpid('0000000000000001'):
                    {'bridge': {'priority': 0x8000}},
                    dpid_lib.str_to_dpid('0000000000000002'):
                    {'bridge': {'priority': 0x9000}},
                    dpid_lib.str_to_dpid('0000000000000003'):
                    {'bridge': {'priority': 0xa000}}}
        self.stp.set_config(config)

        # monitor
        self.mac_ip = {}
        self.datapaths = {}
        self.monitor_thread = hub.spawn(self._monitor)
        self.record = {}

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, ev):
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        # install table-miss flow entry
        #
        # We specify NO BUFFER to max_len of the output action due to
        # OVS bug. At this moment, if we specify a lesser number, e.g.,
        # 128, OVS will send Packet-In with invalid buffer_id and
        # truncated packet data. In that case, we cannot output packets
        # correctly.  The bug has been fixed in OVS v2.1.0.
        match = parser.OFPMatch()
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER,
                                          ofproto.OFPCML_NO_BUFFER)]
        self.add_flow(datapath, 0, match, actions)

    def add_flow(self, datapath, priority, match, actions, buffer_id=None):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS,
                                             actions)]
        if buffer_id:
            mod = parser.OFPFlowMod(datapath=datapath, buffer_id=buffer_id,
                                    priority=priority, match=match,
                                    instructions=inst)
        else:
            mod = parser.OFPFlowMod(datapath=datapath, priority=priority,
                                    match=match, instructions=inst)
        datapath.send_msg(mod)
	
    def delete_flow(self, datapath):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        for dst in self.mac_to_port[datapath.id].keys():
            match = parser.OFPMatch(eth_dst=dst)
            mod = parser.OFPFlowMod(
                datapath, command=ofproto.OFPFC_DELETE,
                out_port=ofproto.OFPP_ANY, out_group=ofproto.OFPG_ANY,
                priority=1, match=match)
            datapath.send_msg(mod)


    #@set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    @set_ev_cls(stplib.EventPacketIn, MAIN_DISPATCHER)
    def _packet_in_handler(self, ev):
        # If you hit this you might want to increase
        # the "miss_send_length" of your switch
        if ev.msg.msg_len < ev.msg.total_len:
            self.logger.debug("packet truncated: only %s of %s bytes",
                              ev.msg.msg_len, ev.msg.total_len)
        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        in_port = msg.match['in_port']

        pkt = packet.Packet(msg.data)
        eth = pkt.get_protocols(ethernet.ethernet)[0]
        ip_v4 = pkt.get_protocols(ipv4.ipv4)
        
        #self.logger.info("packet:")
        #self.logger.info(pkt)
	
        if eth.ethertype == ether_types.ETH_TYPE_LLDP:
            # ignore lldp packet
            return
        dst = eth.dst
        src = eth.src

        dpid = format(datapath.id, "d").zfill(16)

        self.mac_to_port.setdefault(dpid, {})

        # learn a mac address to avoid FLOOD next time.
        self.mac_to_port[dpid][src] = in_port

        if dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][dst]

        else:
            out_port = ofproto.OFPP_FLOOD
	
        actions = [parser.OFPActionOutput(out_port)]
	
        # self.logger.info("packet in %s %s %s %s\n", dpid, src, dst, in_port)
        
        # check IP Protocol and create a match for IP
        if out_port != ofproto.OFPP_FLOOD:
            if eth.ethertype == ether_types.ETH_TYPE_IP:
                ip = pkt.get_protocol(ipv4.ipv4)
                srcip = ip.src
                dstip = ip.dst
                protocol = ip.proto
                self.logger.info("ip : %s to %s, protocol : %s\n", srcip, dstip, protocol)
                if int(srcip[len(srcip)-1]) % 2 != int(dstip[len(dstip)-1]) % 2 :
                    actions = []
                self.logger.info("action = %s\n", actions)
            
                # if ICMP Protocol
                if protocol == in_proto.IPPROTO_ICMP:
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=in_port, ipv4_src=srcip, ipv4_dst=dstip, ip_proto=protocol)
                
                #  if TCP Protocol
                elif protocol == in_proto.IPPROTO_TCP:
                    t = pkt.get_protocol(tcp.tcp)
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=in_port, ipv4_src=srcip, ipv4_dst=dstip, ip_proto=protocol, tcp_src=t.src_port, tcp_dst=t.dst_port)
                
                #  If UDP Protocol 
                elif protocol == in_proto.IPPROTO_UDP:
                    u = pkt.get_protocol(udp.udp)
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=in_port, ipv4_src=srcip, ipv4_dst=dstip, ip_proto=protocol, udp_src=u.src_port, udp_dst=u.dst_port) 
            

                # verify if we have a valid buffer_id, if yes avoid to send both
                # flow_mod & packet_out
                if msg.buffer_id != ofproto.OFP_NO_BUFFER:
                    self.add_flow(datapath, 1, match, actions, msg.buffer_id)
                    return
                else:
                    self.add_flow(datapath, 1, match, actions)

        data = None
        if msg.buffer_id == ofproto.OFP_NO_BUFFER:
            data = msg.data

        out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id,
                                  in_port=in_port, actions=actions, data=data)
        datapath.send_msg(out)
	 
    @set_ev_cls(stplib.EventTopologyChange, MAIN_DISPATCHER)
    def _topology_change_handler(self, ev):
        dp = ev.dp
        dpid_str = dpid_lib.dpid_to_str(dp.id)
        msg = 'Receive topology change event. Flush MAC table.'
        #self.logger.debug("[dpid=%s] %s", dpid_str, msg)

        if dp.id in self.mac_to_port:
            self.delete_flow(dp)
            del self.mac_to_port[dp.id]

    @set_ev_cls(stplib.EventPortStateChange, MAIN_DISPATCHER)
    def _port_state_change_handler(self, ev):
        dpid_str = dpid_lib.dpid_to_str(ev.dp.id)
        of_state = {stplib.PORT_STATE_DISABLE: 'DISABLE',
                    stplib.PORT_STATE_BLOCK: 'BLOCK',
                    stplib.PORT_STATE_LISTEN: 'LISTEN',
                    stplib.PORT_STATE_LEARN: 'LEARN',
                    stplib.PORT_STATE_FORWARD: 'FORWARD'}
        #self.logger.debug("[dpid=%s][port=%d] state=%s",
        #                  dpid_str, ev.port_no, of_state[ev.port_state])

###################################### Monitor #######################################

    @set_ev_cls(ofp_event.EventOFPStateChange, [MAIN_DISPATCHER, DEAD_DISPATCHER])
    def _state_change_handler(self, ev):
        datapath = ev.datapath
        if ev.state == MAIN_DISPATCHER:
            if datapath.id not in self.datapaths:
                self.logger.debug('register datapath: %016x', datapath.id)
                self.datapaths[datapath.id] = datapath
        elif ev.state == DEAD_DISPATCHER:
            if datapath.id in self.datapaths:
                self.logger.debug('unregister datapath: %016x', datapath.id)
                del self.datapaths[datapath.id]

    def _monitor(self):
        while True:
            for dp in self.datapaths.values():
                self._request_stats(dp)
            hub.sleep(10)

    def _request_stats(self, datapath):
        self.logger.debug('send stats request: %016x', datapath.id)
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        req = parser.OFPFlowStatsRequest(datapath)
        datapath.send_msg(req)

        req = parser.OFPPortStatsRequest(datapath, 0, ofproto.OFPP_ANY)
        datapath.send_msg(req)

    @set_ev_cls(ofp_event.EventOFPFlowStatsReply, MAIN_DISPATCHER)
    def _flow_stats_reply_handler(self, ev):
        body = ev.msg.body
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        dpid = ev.msg.datapath.id
        if dpid not in self.record:
            self.record[dpid] = {}

        self.logger.info('\nFlow Statistical Information')
        self.logger.info('datapath         '
                         'in-port  match src_ip src_port       dst_ip dst_port  protocol '
                         'action   packets  bytes')
        self.logger.info('---------------- '
                         '-------- ----------------------------------------------------- '
                         '-------- -------- --------')
	    
        sorted_list = sorted([flow for flow in body if flow.priority == 1],
                           key=lambda flow: (flow.match['ipv4_src'],
                                             flow.match['ipv4_dst']))

        grow_list = []
        group_dict = {}
        new_record = {}
        cnt = 0
        for i, stat in enumerate(sorted_list):
            in_port = out_port = 0
            if stat.match['eth_type'] == ether_types.ETH_TYPE_IP:
                protocol = stat.match['ip_proto']
                # if ICMP Protocol
                if protocol == in_proto.IPPROTO_ICMP:
                    src_port = dst_port = ''
                    proto = 'ICMP'
                #  if TCP Protocol
                elif protocol == in_proto.IPPROTO_TCP:
                    src_port = stat.match['tcp_src']
                    dst_port = stat.match['tcp_dst']
                    proto = 'TCP'
                #  If UDP Protocol 
                elif protocol == in_proto.IPPROTO_UDP:
                    src_port = stat.match['udp_src']
                    dst_port = stat.match['udp_dst']
                    proto = 'UDP'

                if not stat.instructions:
                    self.logger.info('%016x %8x %12s %8s %12s %8s  %8s dropped %8d %8d',
                                    ev.msg.datapath.id, stat.match['in_port'], 
                                    stat.match['ipv4_src'], src_port,
                                    stat.match['ipv4_dst'], dst_port,
                                    proto, stat.packet_count, stat.byte_count)
                    continue
                else:
                    self.logger.info('%016x %8x %12s %8s %12s %8s  %8s port %2x %8d %8d',
                                    ev.msg.datapath.id, stat.match['in_port'], 
                                    stat.match['ipv4_src'], src_port,
                                    stat.match['ipv4_dst'], dst_port,
                                    proto, stat.instructions[0].actions[0].port,
                                    stat.packet_count, stat.byte_count)
                    trans_amount = stat.byte_count
                    in_port = stat.match['in_port']
                    out_port = stat.instructions[0].actions[0].port
                           
                
                if in_port == out_port:
                    continue
                
                new_record[out_port] = trans_amount
                # detect if transfering packet
                if out_port in self.record[dpid]:
                    old_trans_amount = self.record[dpid][out_port]
                    grow = trans_amount - old_trans_amount
                else:
                    grow = trans_amount
                grow_list.append(grow)

                # group different in_port with same out_port
                if out_port not in group_dict:
                    group_dict[out_port] = [cnt]
                else:
                    group_dict[out_port].append(cnt)
                cnt += 1
        
        # detect congestion
        for key in group_dict:
            group = group_dict[key]
            group_grow_list = [grow_list[i] for i in group]
            order = np.argsort(group_grow_list)[::-1]
            growsum = np.sum(group_grow_list)
            threshold = 1000000
            if growsum <= threshold:
                continue
            

            self.logger.info('===============================================================\n')
            self.logger.info('Alert !!! Congestion detected !!!')
            
            
            self.logger.info('datapath         '
                             'in-port  match src_ip src_port       dst_ip dst_port  protocol '
                             'out-port delta_bytes')

            self.logger.info('---------------- '
                             '-------- ----------------------------------------------------- '
                             '-------- -----------')
            
            for idx in order:
                stat = sorted_list[group[idx]]
                grow = group_grow_list[idx]
                protocol = stat.match['ip_proto']
                # if ICMP Protocol
                if protocol == in_proto.IPPROTO_ICMP:
                    src_port = dst_port = ''
                    proto = 'ICMP'
                #  if TCP Protocol
                elif protocol == in_proto.IPPROTO_TCP:
                    src_port = stat.match['tcp_src']
                    dst_port = stat.match['tcp_dst']
                    proto = 'TCP'
                #  If UDP Protocol 
                elif protocol == in_proto.IPPROTO_UDP:
                    src_port = stat.match['udp_src']
                    dst_port = stat.match['udp_dst']
                    proto = 'UDP'
                self.logger.info('%016x %8x %12s %8s %12s %8s  %8s %8x %8d',
                                    ev.msg.datapath.id, stat.match['in_port'], 
                                    stat.match['ipv4_src'], src_port,
                                    stat.match['ipv4_dst'], dst_port,
                                    proto, stat.instructions[0].actions[0].port,
                                    grow)
            self.logger.info('\n===============================================================\n')
            self.logger.info('Drop Big Flow')
            for idx in order:
                stat = sorted_list[group[idx]]
                grow = group_grow_list[idx]
                protocol = stat.match['ip_proto']
                # if ICMP Protocol
                if protocol == in_proto.IPPROTO_ICMP:
                    src_port = dst_port = ''
                    proto = 'ICMP'
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=stat.match['in_port'], ipv4_src=stat.match['ipv4_src'], ipv4_dst=stat.match['ipv4_dst'], ip_proto=protocol)
                #  if TCP Protocol
                elif protocol == in_proto.IPPROTO_TCP:
                    src_port = stat.match['tcp_src']
                    dst_port = stat.match['tcp_dst']
                    proto = 'TCP'
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=stat.match['in_port'], ipv4_src=stat.match['ipv4_src'], ipv4_dst=stat.match['ipv4_dst'], ip_proto=protocol, tcp_src=src_port, tcp_dst=dst_port)
                #  If UDP Protocol 
                elif protocol == in_proto.IPPROTO_UDP:
                    src_port = stat.match['udp_src']
                    dst_port = stat.match['udp_dst']
                    proto = 'UDP'
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=stat.match['in_port'], ipv4_src=stat.match['ipv4_src'], ipv4_dst=stat.match['ipv4_dst'], ip_proto=protocol, udp_src=src_port, udp_dst=dst_port)
                self.logger.info('%016x %8x %12s %8s %12s %8s  %8s %8x %8d',
                                    ev.msg.datapath.id, stat.match['in_port'], 
                                    stat.match['ipv4_src'], src_port,
                                    stat.match['ipv4_dst'], dst_port,
                                    proto, stat.instructions[0].actions[0].port,
                                    grow)
                
                actions = []
                self.add_flow(datapath, 1, match, actions)
                break
            
            self.logger.info('\n===============================================================')
            
        # update record
        for key in new_record:
            self.record[dpid][key] = new_record[key]
        
	    
    @set_ev_cls(ofp_event.EventOFPPortStatsReply, MAIN_DISPATCHER)
    def _port_stats_reply_handler(self, ev):        
	body = ev.msg.body
	
        self.logger.info('\nPort Statistical Information')
        self.logger.info('datapath         port     '
                         'rx-pkts  rx-bytes rx-error '
                         'tx-pkts  tx-bytes tx-error')
        self.logger.info('---------------- -------- '
                         '-------- -------- -------- '
                         '-------- -------- --------')
        for stat in sorted(body, key=attrgetter('port_no')):
            self.logger.info('%016x %8x %8d %8d %8d %8d %8d %8d',
                             ev.msg.datapath.id, stat.port_no,
                             stat.rx_packets, stat.rx_bytes, stat.rx_errors,
                             stat.tx_packets, stat.tx_bytes, stat.tx_errors)


