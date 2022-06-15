const express = require("express");
const bodyParser = require("body-parser");
const {spawn} = require('child_process');
const {exec} = require('child_process');
let app = express();
app.use(bodyParser.urlencoded({extended: true}));

var stringToHTML = function (str) {
    var parser = new DOMParser();
    var doc = parser.parseFromString(str, 'text/html');
    return doc.body;
};

app.get(/\/*/, (req, res) => {
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    let remote_ip = ip.split(":")[3];
    console.log(`${remote_ip} is asking for wifi!`);
    res.setHeader("Context-type", "text/html")
    res.send(`
        <html>
            <form action="login" method="post">
                name: <input type="text" name="name" />
                </br>
                password: <input type="password" name="password" />
                </br>
                <button>GO!</button>
            </form>
        </html>`
        );
});

app.post("/login", (req, res) => {
    console.log(req.body)
    let name = req.body.name;
    let password = req.body.password;
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    let remote_ip = ip.split(":")[3];
    console.log(remote_ip)
    if(name == "cnlab" && password == "mycnlab") {
        res.send("<h1>Loading success </h1>");
        //TODO
	spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "1", "-s", remote_ip, "-j", "ACCEPT"])
	spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "1", "-d", remote_ip, "-j", "ACCEPT"])
	spawn("iptables", ["-I", "FORWARD", "-s", remote_ip, "-j", "ACCEPT"])
	spawn("iptables", ["-I", "FORWARD", "-d", remote_ip, "-j", "ACCEPT"])

        
    } else {
        res.send("<h1>Error</h1>")
    }
});

app.listen(9090);
console.log("Start listening!")
