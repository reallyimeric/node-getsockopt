const getSockOpt = require('bindings')('getSockOpt.node');
// eslint-disable-next-line @typescript-eslint/no-var-requires
const assert = require('assert');
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { Socket, Server } = require('net');

const server = new Server();
server.on('listening', () => {
    const { port } = server.address();
    const socket = new Socket();
    const connArr = [];
    server.on('connection', (csock) => {
        connArr.push(csock);
    });
    socket.on('connect', () => {
        getSockOpt.getOriginalDst(socket, (err, fd) => {
            assert.strict(Number.isInteger(fd));
            server.close();
            connArr.forEach((sock) => sock.destroy());
        });
    });
    socket.connect(port, 'localhost');
});
server.listen();
