#include "tassadar.hpp"

#include <boost/program_options.hpp>
#include <iostream>

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>


namespace po = boost::program_options;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using ::apache::thrift::stdcxx::shared_ptr;

int main(int argc, char **argv) {
  int port, worker_count;
  po::options_description desc("Options");
  desc.add_options()
    ("help", "show help message")
    ("port,P", po::value<int>(&port)->default_value(9090), "set port")
    ("worker", po::value<int>(&worker_count)->default_value(100), "set worker count");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  shared_ptr<TassadarServerHandler> handler(new TassadarServerHandler());
  shared_ptr<TProcessor> processor(new TassadarServerProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();

  return 0;
}
