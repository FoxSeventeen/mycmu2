//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// count_min_sketch.cpp
//
// Identification: src/primer/count_min_sketch.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/count_min_sketch.h"

#include <stdexcept>
#include <string>

namespace bustub {

/**
 * count-min sketch 的构造函数。
 *
 * @param width sketch 矩阵的宽度。
 * @param depth sketch 矩阵的深度。
 * @throws std::invalid_argument 当 width 或 depth 为 0 时抛出。
 */
template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(uint32_t width, uint32_t depth) : width_(width), depth_(depth) {
  /** @TODO(student) 请实现该函数！ Finished*/
  if(width_==0||depth_==0)
  {
    throw std::invalid_argument("width and depth must be greater than 0");
  }
  sketch_.reserve(depth_);
  for(uint32_t i=0;i<depth_;i++)
  {
    std::vector<std::atomic<uint32_t>> row(width_);
    sketch_.push_back(std::move(row));
    for(uint32_t j=0;j<width_;j++)
    {
      auto &row=sketch_.back();
      for(uint32_t j=0;j<width_;j++)
      {
        row[j].store(0,std::memory_order_relaxed);
      }
    }
  }

  /** @spring2026 请不要修改以下内容 */
  // 初始化带种子的哈希函数
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(CountMinSketch &&other) noexcept : width_(other.width_), depth_(other.depth_),sketch_(std::move(other.sketch_)){
  /** @TODO(student) 请实现该函数！ Finished*/
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::operator=(CountMinSketch &&other) noexcept -> CountMinSketch & {
  /** @TODO(student) 请实现该函数！ Finished*/
  this->width_=other.width_;this->depth_=other.depth_;
  this->sketch_=std::move(other.sketch_);
  this->hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    this->hash_functions_.push_back(this->HashFunction(i));
  }
  return *this;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Insert(const KeyType &item) {
  /** @TODO(student) 请实现该函数！ Finished*/
  for(uint32_t i=0;i<depth_;i++)
  {
    size_t cur=hash_functions_[i](item);
    sketch_[i][cur].fetch_add(1,std::memory_order_relaxed);
  }
}

template <typename KeyType>
void CountMinSketch<KeyType>::Merge(const CountMinSketch<KeyType> &other) {
  /** @TODO(student) 请实现该函数！ Finished*/
  if (width_ != other.width_ || depth_ != other.depth_) {
    throw std::invalid_argument("Incompatible CountMinSketch dimensions for merge.");
  }
  for(uint32_t i=0;i<depth_;i++)
  {
    for(uint32_t j=0;j<width_;j++)
    {
      sketch_[i][j].fetch_add(other.sketch_[i][j],std::memory_order_relaxed);
    }
  }
  
}

template <typename KeyType>
auto CountMinSketch<KeyType>::Count(const KeyType &item) const -> uint32_t {
  /** @TODO(student) 请实现该函数！ finished*/
  uint32_t Min=std::numeric_limits<uint32_t>::max();
  for(uint32_t i=0;i<depth_;i++)
  {
    Min=std::min(Min,sketch_[i][hash_functions_[i](item)].load(std::memory_order_relaxed));
  }
  return Min;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Clear() {
  /** @TODO(student) 请实现该函数！ finished*/
  for(uint32_t i=0;i<depth_;i++)
  {
    for(uint32_t j=0;j<width_;j++)
    {
      sketch_[i][j].store(0,std::memory_order_relaxed);
    }
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::TopK(uint16_t k, const std::vector<KeyType> &candidates)
    -> std::vector<std::pair<KeyType, uint32_t>> {
  /** @TODO(student) 请实现该函数！ */
  std::vector<std::pair<KeyType,uint32_t>>result;
  result.reserve(candidates.size());
  for(auto &item: candidates)
  {
    result.emplace_back(item,Count(item));
  }
  std::sort(result.begin(),result.end(),[](const auto &a,const auto &b)-> bool {
    return a.second > b.second;
  });
  if(result.size()>k)result.resize(k);
  return result;
}

// 为测试中使用的所有类型进行显式实例化
template class CountMinSketch<std::string>;
template class CountMinSketch<int64_t>;  // 用于 int64_t 测试
template class CountMinSketch<int>;      // 同时覆盖 int 和 int32_t
}  // namespace bustub
