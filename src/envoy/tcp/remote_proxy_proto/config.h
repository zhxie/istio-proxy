#pragma once

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Network {
namespace RemoteProxyProto {

class RemoteProxyProtoFilter : public Network::ListenerFilter {
 public:
  RemoteProxyProtoFilter(const std::string& destination_ip)
      : destination_ip_(destination_ip) {}

  // Network::ListenerFilter
  Network::FilterStatus onAccept(Network::ListenerFilterCallbacks& cb) override;

 private:
  const std::string destination_ip_;
};

class RemoteProxyProtoFilterConfigFactory
    : public Server::Configuration::NamedListenerFilterConfigFactory {
 public:
  // NamedListenerFilterConfigFactory
  Network::ListenerFilterFactoryCb createListenerFilterFactoryFromProto(
      const Protobuf::Message& message,
      const Network::ListenerFilterMatcherSharedPtr& listener_filter_matcher,
      Server::Configuration::ListenerFactoryContext& context) override;
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
  std::string name() const override { return "istio.remote_proxy_proto"; }
};

}  // namespace RemoteProxyProto
}  // namespace Network
}  // namespace Envoy
