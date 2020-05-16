/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _LINKER_CONFIG_H_
#define _LINKER_CONFIG_H_

#include <android/api-level.h>

#include <stdlib.h>
#include <limits.h>
#include "private/bionic_macros.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

class NamespaceLinkConfig {
 public:
  NamespaceLinkConfig() = default;
  NamespaceLinkConfig(const std::string& ns_name, const std::string& shared_libs)
      : ns_name_(ns_name), shared_libs_(shared_libs)  {}

  const std::string& ns_name() const {
    return ns_name_;
  }

  const std::string& shared_libs() const {
    return shared_libs_;
  }

 private:
  std::string ns_name_;
  std::string shared_libs_;
};

class NamespaceConfig {
 public:
  explicit NamespaceConfig(const std::string& name)
      : name_(name), isolated_(false), visible_(false)
  {}

  const char* name() const {
    return name_.c_str();
  }

  bool isolated() const {
    return isolated_;
  }

  bool visible() const {
    return visible_;
  }

  const std::vector<std::string>& search_paths() const {
    return search_paths_;
  }

  const std::vector<std::string>& permitted_paths() const {
    return permitted_paths_;
  }

  const std::vector<NamespaceLinkConfig>& links() const {
    return namespace_links_;
  }

  void add_namespace_link(const std::string& ns_name, const std::string& shared_libs) {
    namespace_links_.push_back(NamespaceLinkConfig(ns_name, shared_libs));
  }

  void set_isolated(bool isolated) {
    isolated_ = isolated;
  }

  void set_visible(bool visible) {
    visible_ = visible;
  }

  void set_search_paths(std::vector<std::string>&& search_paths) {
    search_paths_ = search_paths;
  }

  void set_permitted_paths(std::vector<std::string>&& permitted_paths) {
    permitted_paths_ = permitted_paths;
  }
 private:
  const std::string name_;
  bool isolated_;
  bool visible_;
  std::vector<std::string> search_paths_;
  std::vector<std::string> permitted_paths_;
  std::vector<NamespaceLinkConfig> namespace_links_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NamespaceConfig);
};

class Config {
 public:
  Config() : target_sdk_version_(__ANDROID_API__) {}

  const std::vector<std::unique_ptr<NamespaceConfig>>& namespace_configs() const {
    return namespace_configs_;
  }

  const NamespaceConfig* default_namespace_config() const {
    auto it = namespace_configs_map_.find("default");
    return it == namespace_configs_map_.end() ? nullptr : it->second;
  }

  uint32_t target_sdk_version() const {
    return target_sdk_version_;
  }

  // note that this is one time event and therefore there is no need to
  // read every section of the config. Every linker instance needs at
  // most one configuration.
  // Returns false in case of an error. If binary config was not found
  // sets *config = nullptr.
  static bool read_binary_config(const char* ld_config_file_path,
                                 const char* binary_realpath,
                                 bool is_asan,
                                 const Config** config,
                                 std::string* error_msg);
 private:
  void clear();

  void set_target_sdk_version(uint32_t target_sdk_version) {
    target_sdk_version_ = target_sdk_version;
  }

  NamespaceConfig* create_namespace_config(const std::string& name);

  std::vector<std::unique_ptr<NamespaceConfig>> namespace_configs_;
  std::unordered_map<std::string, NamespaceConfig*> namespace_configs_map_;
  uint32_t target_sdk_version_;

  DISALLOW_COPY_AND_ASSIGN(Config);
};

#endif /* _LINKER_CONFIG_H_ */
