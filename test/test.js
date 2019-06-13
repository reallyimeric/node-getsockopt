const getSockOpt = require('bindings')('getSockOpt.node');
// eslint-disable-next-line @typescript-eslint/no-var-requires
const assert = require('assert');
// eslint-disable-next-line @typescript-eslint/no-var-requires
const { Socket, Server } = require('net');

// TODO: need redir
const server = new Server();
server.on('listening', () => {
    const { port } = server.address();
    const socket = new Socket();
    server.on('connection', (csock) => {
        getSockOpt.getOriginalDst(csock, (err, result) => {
            assert.strict(result.family === 'IPv4');
            assert.strict(result.address === '127.0.0.1');
            assert.strict(result.port === port);
            server.close();
            csock.destroy();
        });
    });
    socket.connect(port, '127.0.0.1');
});
server.listen();
