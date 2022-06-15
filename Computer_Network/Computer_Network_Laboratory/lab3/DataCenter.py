import sys
from mininet.topo import Topo
 
class DataCenter(Topo):
    "Simple topology example."
 
    def __init__(self,coreCount = 2, edgeCount = 2):
        "Create custom topo."
 
        # Initialize topology
        Topo.__init__(self)

        cores = []
        edges = []
        hosts = []

        for i in range(coreCount):
            name = 's' + str(i)
            c = self.addSwitch(name)
            cores.append(c)

        for i in range(4 * edgeCount):
            name = 's' + str(coreCount + i)
            s = self.addSwitch(name)
            edges.append(s)

        for i in range(4 * edgeCount):
            name = 'h' + str(i)
            h = self.addHost(name)
            hosts.append(h)

        for i in range(coreCount):
            if(i % 2 == 0):
                for j in range(edgeCount):
                    self.addLink(cores[i], edges[4 * j])
            else:
                for j in range(edgeCount):
                    self.addLink(cores[i], edges[4 * j + 1])

        for i in range(edgeCount):
            self.addLink(edges[4 * i], edges[4 * i + 2])
            self.addLink(edges[4 * i], edges[4 * i + 3])
            self.addLink(edges[4 * i + 1], edges[4 * i + 2])
            self.addLink(edges[4 * i + 1], edges[4 * i + 3])
            self.addLink(edges[4 * i + 2], hosts[4 * i])
            self.addLink(edges[4 * i + 2], hosts[4 * i + 1])
            self.addLink(edges[4 * i + 3], hosts[4 * i + 2])
            self.addLink(edges[4 * i + 3], hosts[4 * i + 3])

 
topos = { 'dataCenter': DataCenter }
