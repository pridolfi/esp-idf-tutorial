#!/usr/bin/env python3

from socket import socket
from ssl import SSLCertVerificationError, SSLContext, PROTOCOL_TLS_SERVER, CERT_REQUIRED

ip = '0.0.0.0'
port = 8443
context = SSLContext(PROTOCOL_TLS_SERVER)
context.load_cert_chain('main/server.crt', 'main/server.key')
context.verify_mode = CERT_REQUIRED

context.load_verify_locations('main/esp32.crt')

def search_key(key, tuples):
    for k in tuples:
        if isinstance(k, tuple):
            if k[0] == key:
                return k[1]
            else:
                return search_key(key, k)
        else:
            return None
    return None


def get_common_name(cert):
    subject = cert['subject']
    return search_key('commonName', subject)


with socket() as server:
    server.bind((ip, port))
    print(f"Listening on {(ip, port)}")
    server.listen(1)
    while True:
        print('Waiting for connection...')
        connection, address = server.accept()
        try:
            with context.wrap_socket(connection, server_side=True) as tls:
                peername = get_common_name(tls.getpeercert())
                print(f'Incoming connection from {address}.')
                print(f'Client peername is {peername}')
                data = tls.recv(1024)
                print(f'Client Says: {data}. Sending response...')
                tls.sendall(b"Hi! How are you?")
        except SSLCertVerificationError as ex:
            print(ex) 
  