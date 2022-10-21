from socket import create_connection
from ssl import create_default_context, PROTOCOL_TLS_CLIENT

hostname='server'
ip = '127.0.0.1'
port = 8443
context = create_default_context()
context.load_cert_chain('main/esp32.crt', 'main/esp32.key')
context.load_verify_locations('main/server.crt')

with create_connection((ip, port)) as client:
    with context.wrap_socket(client, server_hostname=hostname) as tls:
        print(f'Using {tls.version()}\n')
        tls.sendall(b'Hello, world')

        data = tls.recv(1024)
        print(f'Server says: {data}')