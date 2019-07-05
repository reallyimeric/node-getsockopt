import { Socket } from 'net';

export type SocketAddress = {
    family: 'IPv4' | 'IPv6';
    address: string;
    port: number;
}

export function getOriginalDst(socket: Socket): SocketAddress;

declare const getSockOpt: {
    getOriginalDst: typeof getOriginalDst;
};

export default getSockOpt;
