#include "cachestat_ebpf.h"

#include <bcc_syms.h>


leveldb::Cachestat_eBPF::Cachestat_eBPF()
{
    auto init_res = bpf_.init(cache_ebpf::BPF_PROGRAM);
      if (init_res.code() != 0)
          std::cout << init_res.msg() << std::endl;
      //attach_kernel_probe_event();
      //struct cache_info cif=  get_cache_info();
      //detach_kernel_probe_event();

}

void leveldb::Cachestat_eBPF:: attach_kernel_probe_event()
{
    bpf_.attach_kprobe("add_to_page_cache_lru", "do_count");
    bpf_.attach_kprobe("mark_page_accessed", "do_count");
    bpf_.attach_kprobe("account_page_dirtied", "do_count");
    bpf_.attach_kprobe("mark_buffer_dirty", "do_count");

}

void leveldb::Cachestat_eBPF::detach_kernel_probe_event()
{
    bpf_.detach_kprobe("add_to_page_cache_lru");
    bpf_.detach_kprobe("mark_page_accessed");
    bpf_.detach_kprobe("account_page_dirtied");
    bpf_.detach_kprobe("mark_buffer_dirty");
}

leveldb::cache_info leveldb::Cachestat_eBPF::get_cache_info()
{
    struct cache_info info;
    auto cache_hash_table = bpf_.get_hash_table<struct key_t, uint64_t>("counts");
    std::cout<< "get into leveldb::cache_info leveldb::Cachestat_eBPF::get_cache_info()"<<std::endl;
    for (auto it: cache_hash_table.get_table_offline()) {
        std::string pid_name = it.first.comm;
        std::cout <<"The pid name: "<< pid_name <<std::endl;//cyf add
        //this if is used to judge whether the process belongs to ycsb
        if(pid_name.find("db_bennch") >= 0){
            struct bcc_symbol b_symbol;

            struct bpf_stack_build_id *bsb_id = (struct bpf_stack_build_id*)(it.first.ip);

            int res = bcc_buildsymcache_resolve(bpf_.get_bsymcache(), bsb_id, &b_symbol);
            std::cout << b_symbol.name<<std::endl;



        }
    }



}
