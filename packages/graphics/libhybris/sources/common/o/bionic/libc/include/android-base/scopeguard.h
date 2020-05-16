/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ANDROID_BASE_SCOPEGUARD_H
#define ANDROID_BASE_SCOPEGUARD_H
#include <utility>  // for std::move
namespace android {
namespace base {
template <typename F>
class ScopeGuard {
 public:
  ScopeGuard(F f) : f_(f), active_(true) {}
  ScopeGuard(ScopeGuard&& that) : f_(std::move(that.f_)), active_(that.active_) {
    that.active_ = false;
  }
  ~ScopeGuard() {
    if (active_) f_();
  }
  ScopeGuard() = delete;
  ScopeGuard(const ScopeGuard&) = delete;
  void operator=(const ScopeGuard&) = delete;
  void operator=(ScopeGuard&& that) = delete;
  void Disable() { active_ = false; }
  bool active() const { return active_; }
 private:
  F f_;
  bool active_;
};
template <typename T>
ScopeGuard<T> make_scope_guard(T f) {
  return ScopeGuard<T>(f);
}
}  // namespace base
}  // namespace android
#endif  // ANDROID_BASE_SCOPEGUARD_H
