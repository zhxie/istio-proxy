/* Copyright 2018 Istio Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "src/envoy/tcp/remote_proxy_proto/config.h"

#include "envoy/registry/registry.h"
#include "src/envoy/tcp/remote_proxy_proto/config.pb.h"

namespace Envoy {
namespace Network {
namespace RemoteProxyProto {

Network::FilterFactoryCb
RemoteProxyProtoNetworkFilterConfigFactory::createFilterFactoryFromProto(
    const Protobuf::Message&, Server::Configuration::FactoryContext&) {
  return [](Network::FilterManager& filter_manager) -> void {
    filter_manager.addReadFilter(
        std::make_shared<RemoteProxyProtoFilter>());
  };
}

ProtobufTypes::MessagePtr
RemoteProxyProtoNetworkFilterConfigFactory::createEmptyConfigProto() {
  return std::make_unique<io::istio::tcp::remote_proxy_proto::v1::Config>();
}

/**
 * Static registration for the forward_original_sni filter. @see
 * RegisterFactory.
 */
static Registry::RegisterFactory<
    RemoteProxyProtoNetworkFilterConfigFactory,
    Server::Configuration::NamedNetworkFilterConfigFactory>
    registered_;

}  // namespace RemoteProxyProto
}  // namespace Network
}  // namespace Envoy
