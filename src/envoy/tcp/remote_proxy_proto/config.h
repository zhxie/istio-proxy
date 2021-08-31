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

#pragma once

#include "envoy/server/filter_config.h"
#include "source/common/network/proxy_protocol_filter_state.h"

namespace Envoy {
namespace Network {
namespace RemoteProxyProto {

class RemoteProxyProtoFilter : public Network::ReadFilter {
 public:
  RemoteProxyProtoFilter() {}

  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance&,
                               bool) override {
    return Network::FilterStatus::Continue;
  }
  Network::FilterStatus onNewConnection() override {
    read_callbacks_->connection().streamInfo().filterState()->setData(ProxyProtocolFilterState::key(), std::make_shared<ProxyProtocolFilterState>(ProxyProtocolData{
          read_callbacks_->connection().addressProvider().localAddress(),
          read_callbacks_->connection().addressProvider().remoteAddress()}),
        StreamInfo::FilterState::StateType::Mutable, StreamInfo::FilterState::LifeSpan::Connection);
    return Network::FilterStatus::Continue;
  }
  void initializeReadFilterCallbacks(
      Network::ReadFilterCallbacks& callbacks) override {
    read_callbacks_ = &callbacks;
  }

 private:
  Network::ReadFilterCallbacks* read_callbacks_{};
};

class RemoteProxyProtoNetworkFilterConfigFactory
    : public Server::Configuration::NamedNetworkFilterConfigFactory {
 public:
  // NamedNetworkFilterConfigFactory
  Network::FilterFactoryCb createFilterFactoryFromProto(
      const Protobuf::Message&,
      Server::Configuration::FactoryContext&) override;
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
  std::string name() const override { return "istio.remote_proxy_proto"; }
};

}  // namespace RemoteProxyProto
}  // namespace Network
}  // namespace Envoy
