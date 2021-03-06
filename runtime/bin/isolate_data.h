// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef RUNTIME_BIN_ISOLATE_DATA_H_
#define RUNTIME_BIN_ISOLATE_DATA_H_

#include "include/dart_api.h"
#include "platform/assert.h"
#include "platform/globals.h"

namespace dart {

// Forward declaration.
template <typename T>
class MallocGrowableArray;

}  // namespace dart

namespace dart {
namespace bin {

// Forward declaration.
class AppSnapshot;
class EventHandler;
class Loader;

// Data associated with every isolate in the standalone VM
// embedding. This is used to free external resources for each isolate
// when the isolate shuts down.
class IsolateData {
 public:
  IsolateData(const char* url,
              const char* package_root,
              const char* packages_file,
              AppSnapshot* app_snapshot);
  ~IsolateData();

  Dart_Handle builtin_lib() const {
    ASSERT(builtin_lib_ != NULL);
    ASSERT(!Dart_IsError(builtin_lib_));
    return builtin_lib_;
  }
  void set_builtin_lib(Dart_Handle lib) {
    ASSERT(builtin_lib_ == NULL);
    ASSERT(lib != NULL);
    ASSERT(!Dart_IsError(lib));
    builtin_lib_ = Dart_NewPersistentHandle(lib);
  }

  char* script_url;
  char* package_root;
  char* packages_file;

  const uint8_t* kernel_buffer() const { return kernel_buffer_; }
  intptr_t kernel_buffer_size() const { return kernel_buffer_size_; }
  void set_kernel_buffer(uint8_t* buffer, intptr_t size, bool take_ownership) {
    ASSERT(kernel_buffer_ == NULL);
    kernel_buffer_ = buffer;
    kernel_buffer_size_ = size;
    owns_kernel_buffer_ = take_ownership;
  }

  void UpdatePackagesFile(const char* packages_file_) {
    if (packages_file != NULL) {
      free(packages_file);
      packages_file = NULL;
    }
    packages_file = strdup(packages_file_);
  }

  const char* resolved_packages_config() const {
    return resolved_packages_config_;
  }

  void set_resolved_packages_config(const char* packages_config) {
    if (resolved_packages_config_ != NULL) {
      free(resolved_packages_config_);
      resolved_packages_config_ = NULL;
    }
    resolved_packages_config_ = strdup(packages_config);
  }

  // While loading a loader is associated with the isolate.
  bool HasLoader() const { return loader_ != NULL; }
  Loader* loader() const {
    ASSERT(loader_ != NULL);
    return loader_;
  }
  void set_loader(Loader* loader) {
    ASSERT((loader_ == NULL) || (loader == NULL));
    loader_ = loader;
  }
  MallocGrowableArray<char*>* dependencies() const { return dependencies_; }
  void set_dependencies(MallocGrowableArray<char*>* deps) {
    dependencies_ = deps;
  }

  void OnIsolateShutdown();

 private:
  Dart_Handle builtin_lib_;
  Loader* loader_;
  AppSnapshot* app_snapshot_;
  MallocGrowableArray<char*>* dependencies_;
  char* resolved_packages_config_;
  uint8_t* kernel_buffer_;
  intptr_t kernel_buffer_size_;
  bool owns_kernel_buffer_;

  DISALLOW_COPY_AND_ASSIGN(IsolateData);
};

}  // namespace bin
}  // namespace dart

#endif  // RUNTIME_BIN_ISOLATE_DATA_H_
