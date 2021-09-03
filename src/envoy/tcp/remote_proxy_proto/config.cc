#include "src/envoy/tcp/remote_proxy_proto/config.h"

#include "envoy/registry/registry.h"
#include "source/common/network/utility.h"
#include "source/common/protobuf/utility.h"
#include "src/envoy/tcp/remote_proxy_proto/config.pb.h"
#include "src/envoy/tcp/remote_proxy_proto/config.pb.validate.h"

namespace Envoy {
namespace Network {
namespace RemoteProxyProto {

Network::FilterStatus RemoteProxyProtoFilter::onAccept(
    Network::ListenerFilterCallbacks& cb) {
  Network::ConnectionSocket& socket = cb.socket();
  // Construct the new address based on the provided config
  if (socket.addressType() == Network::Address::Type::Ip &&
      socket.addressProvider().localAddress()->ip()->addressAsString() !=
          destination_ip_) {
    socket.addressProvider().restoreLocalAddress(
        Network::Utility::parseInternetAddress(
            destination_ip_,
            socket.addressProvider().localAddress()->ip()->port()));
  }

  return Network::FilterStatus::Continue;
}
Network::ListenerFilterFactoryCb
RemoteProxyProtoFilterConfigFactory::createListenerFilterFactoryFromProto(
    const Protobuf::Message& message,
    const Network::ListenerFilterMatcherSharedPtr& listener_filter_matcher,
    Server::Configuration::ListenerFactoryContext& context) {
  const auto& proto_config = Envoy::MessageUtil::downcastAndValidate<
      const io::istio::tcp::remote_proxy_proto::v1::Config&>(
      message, context.messageValidationVisitor());
  return [listener_filter_matcher, proto_config](Network::ListenerFilterManager& filter_manager) -> void {
    filter_manager.addAcceptFilter(listener_filter_matcher,
                                   std::make_unique<RemoteProxyProtoFilter>(
                                       proto_config.destination_ip()));
  };
}

ProtobufTypes::MessagePtr
RemoteProxyProtoFilterConfigFactory::createEmptyConfigProto() {
  return std::make_unique<io::istio::tcp::remote_proxy_proto::v1::Config>();
}

/**
 * Static registration for the forward_original_sni filter. @see
 * RegisterFactory.
 */
static Registry::RegisterFactory<
    RemoteProxyProtoFilterConfigFactory,
    Server::Configuration::NamedListenerFilterConfigFactory>
    registered_;

}  // namespace RemoteProxyProto
}  // namespace Network
}  // namespace Envoy
