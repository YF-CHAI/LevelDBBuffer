// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/options.h"

#include "leveldb/comparator.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include <iostream>

namespace leveldb {

Options::Options()
    : comparator(BytewiseComparator()),
      create_if_missing(false),
      error_if_exists(false),
      paranoid_checks(false),
      env(Env::Default()),
      info_log(NULL),
      write_buffer_size(64<<20),//cyf changed default:4MB
      max_open_files(5000),
      block_cache(NULL),
      block_size(4096),
      block_restart_interval(16),
      max_file_size(32<<20),//cyf changed default:2MB
      compression(kSnappyCompression),
      reuse_logs(false),
      filter_policy(NewBloomFilterPolicy(128)),
      amplify(5.0),
      top_level_size(128.0*1048576.0){//cyf changed default: 10MB
          //std::cout<<"options:filter:"<<filter_policy<<std::endl;
}

}  // namespace leveldb
