// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/options.h"
#include "db/dbformat.h"
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
      write_buffer_size(config::kLDCMaxWriteBufferSize),//cyf changed default:4MB
      max_open_files(1000),
      block_cache(NULL),
      block_size(4096),//cyf change default 4096
      block_restart_interval(16),
      max_file_size(config::kLDCMaxFileSizeLimit),//cyf changed default:2MB
      compression(kSnappyCompression),//cyf change for test, default:kSnappyCompression
      reuse_logs(false),
      filter_policy(NewBloomFilterPolicy(8)),
      //filter_policy(nullptr),
      amplify(10.0),
      top_level_size(20.0 * 1048576.0){//cyf changed default: 10MB
          //std::cout<<"options:filter:"<<filter_policy<<std::endl;
}

}  // namespace leveldb
