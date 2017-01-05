#include "db/buffer_iterator.h"
#include "db/version_set.h"
#include "db/table_cache.h"
#include <iostream>

namespace leveldb{


BufferNodeIterator::BufferNodeIterator(const ReadOptions& options,VersionSet* vset,BufferNode* node)
  :buffernode_(node), 
  iterator_(vset->ssd_table_cache_->NewIterator(options, node->number, node->filesize)),
  icmp_(&(vset->icmp_)){
      //std::cout<<"buffernode iterator build"<<std::endl;
      //std::cout<<"buffernode iterator file size="<<node->filesize<<std::endl;
      //std::cout<<"node->number="<<node->number<<std::endl;
      //if(buffernode_->smallest.Rep().size()>0)
        //std::cout<<"node->smallest="<<node->smallest.Rep()<<std::endl;
      //else
         //::cout<<"node->smallest=0"<<std::endl;
      SeekToFirst();
      }
  


bool BufferNodeIterator::Valid() const {
    //std::cout<<"buffer node iterator valid node number"<<buffernode_->number<<std::endl;
    //std::cout<<"buffer node iterator valid smallest:"<<buffernode_->smallest.Rep()<<std::endl;
    if (!iterator_->Valid()){
      //std::cout<<"buffer node iterator not valid"<<std::endl;
      return false; 
  } 
  
  //std::cout<<"buffer node iterator valid"<<std::endl;     
  Slice k = iterator_->key();

  
  bool result;
  result = (buffernode_->smallest.Rep().size()==0 
  || icmp_->Compare(k,buffernode_->smallest.Encode()) > 0 )
      && (icmp_->Compare(k,buffernode_->largest.Encode()) <= 0);
  
  //std::cout<<"buffer node iterator valid result"<<result<<std::endl; 
  if(icmp_->Compare(k,buffernode_->largest.Encode()) > 0){
      //std::cout<<"buffer node iterator valid stop"<<std::endl;
  }
  return result;
}

Slice BufferNodeIterator::key() const {
  assert(Valid());
  return iterator_->key();
}


void BufferNodeIterator::Next() {
  if(Valid())
  iterator_->Next();
}



void BufferNodeIterator::Prev() {
  if(Valid())
  iterator_->Prev();
}


Status BufferNodeIterator::status() const {
  return Status::OK(); 
}

void BufferNodeIterator::Seek(const Slice& target) {
  /*
  if (!iterator_->Valid() || (icmp_->Compare(target,buffernode_->largest.Encode()) > 0)){
      SeekToLast();
      return ;
  }
    
  if (buffernode_->smallest.Rep().size()>0 && 
  icmp_->Compare(target,buffernode_->smallest.Encode()) <= 0){
      SeekToFirst();
      return ;
  }
  */
  iterator_->Seek(target);
    
}

void BufferNodeIterator::SeekToFirst() {
  if(buffernode_->smallest.Rep().size()==0){
      iterator_->SeekToFirst();
      return;
  }
      
  iterator_->Seek((buffernode_->smallest).Encode()); 
  assert(iterator_->Valid());
  if(icmp_->Compare(iterator_->key(),buffernode_->smallest.Encode())==0)
      iterator_->Next();
  kkey = iterator_->key().ToString();
}
  
void BufferNodeIterator::SeekToLast() {
  iterator_->Seek((buffernode_->largest).Encode()); 
}


Slice BufferNodeIterator::value() const {
  return iterator_->value();
}

BufferNodeIterator::~BufferNodeIterator(){
    //std::cout<<"buffer node iterator release"<<std::endl;
    delete iterator_;
}

Iterator* NewBufferIterator(const ReadOptions& options,VersionSet* vset,Buffer* buffer){
    
     int space = buffer->nodes.size();
     Iterator** list = new Iterator*[space];
     assert(buffer!=NULL);
     //std::cout<<"buffer iterator build"<<std::endl;
     for(int i=0;i<buffer->nodes.size();i++){
     	 BufferNodeIterator* ptr = new BufferNodeIterator(options,vset,&(buffer->nodes[i]));
     	 //list.push_back(ptr);
         list[i] = ptr;
     }
     Iterator* buffer_iter =
      NewMergingIterator(&(vset->icmp_), list, space);
      
      delete[] list;
      
      return buffer_iter;
    }


} // namespace leveldb