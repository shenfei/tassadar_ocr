from thrift import Thrift
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol

from .gen_py.tassadar import TassadarServer


class TassadarClient(object):

    def __init__(self, host='localhost', port=9090):
        self.host = host
        self.port = port
        try:
            self.transport = TSocket.TSocket(self.host, self.port)
            self.transport = TTransport.TBufferedTransport(self.transport)
            protocol = TBinaryProtocol.TBinaryProtocol(self.transport)
            self.client = TassadarServer.Client(protocol)
            self.transport.open()
        except Thrift.TException as tx:
            print(f'{tx.message}')

    def version(self):
        _version = ''
        try:
            _version = self.client.version()
        except Exception as e:
            print(e)
        return _version

    def get_ocr(self, image):
        ocr_text = None
        try:
            ocr_text = self.client.get_ocr(image)
        except Exception as e:
            print(e)
        return ocr_text

    def line_ocr(self, image):
        ocr_text = None
        try:
            ocr_text = self.client.line_ocr(image)
        except Exception as e:
            print(e)
        return ocr_text

    def cut_image(self, image, cut_type):
        sub_images = []
        try:
            sub_images = self.client.cut_image(image, cut_type)
        except Exception as e:
            print(e)
        return sub_images
