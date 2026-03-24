//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// count_min_sketch.h
//
// Identification: src/include/primer/count_min_sketch.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include <atomic>

#include "common/util/hash_util.h"

namespace bustub {

template <typename KeyType>
class CountMinSketch {
 public:
  /** @brief 使用指定维度构造 count-min sketch
   * @param width 桶的数量
   * @param depth 哈希函数的数量
   */
  explicit CountMinSketch(uint32_t width, uint32_t depth);

  CountMinSketch() = delete;                                            // 已删除默认构造函数
  CountMinSketch(const CountMinSketch &) = delete;                      // 已删除拷贝构造函数
  auto operator=(const CountMinSketch &) -> CountMinSketch & = delete;  // 已删除拷贝赋值运算符

  CountMinSketch(CountMinSketch &&other) noexcept;                      // 移动构造函数
  auto operator=(CountMinSketch &&other) noexcept -> CountMinSketch &;  // 移动赋值运算符

  /**
   * @brief 向 count-min sketch 中插入一个元素
   *
   * @param item 要增加计数的元素
   * @note 会同时更新最小堆
   */
  void Insert(const KeyType &item);

  /**
   * @brief 获取某个元素的估计计数
   *
   * @param item 要查询的元素
   * @return 估计计数值
   */
  auto Count(const KeyType &item) const -> uint32_t;

  /**
   * @brief 将 sketch 重置为空的初始状态
   *
   * @note 会清空 sketch 矩阵、元素集合以及 top-k 最小堆
   */
  void Clear();

  /**
   * @brief 将当前 CountMinSketch 与另一个进行合并，并使用两者的组合数据更新当前 sketch。
   *
   * @param other 要合并的另一个 CountMinSketch。
   * @throws std::invalid_argument 当两个 sketch 的维度不兼容时抛出。
   */
  void Merge(const CountMinSketch<KeyType> &other);

  /**
   * @brief 从候选列表中根据估计计数获取前 k 个元素。
   *
   * @param k 要返回的前 k 个元素数量（会被限制在初始 k 的范围内）
   * @param candidates 用于参与 top k 计算的候选元素列表
   * @return 按计数降序排列的 (元素, 计数) 对向量
   */
  auto TopK(uint16_t k, const std::vector<KeyType> &candidates) -> std::vector<std::pair<KeyType, uint32_t>>;

 private:
  /** count-min sketch 矩阵的维度 */
  uint32_t width_;  // 每个哈希函数对应的桶数量
  uint32_t depth_;  // 独立哈希函数的数量
  /** 为每一行预先计算好的哈希函数 */
  std::vector<std::vector<std::atomic<uint32_t>>> sketch_;//矩阵
  std::vector<std::function<size_t(const KeyType &)>> hash_functions_;

  /** @spring2026 请不要修改以下内容 */
  constexpr static size_t SEED_BASE = 15445;

  /**
   * @brief 带种子的哈希函数生成器
   *
   * @param seed 用于创建彼此独立的哈希函数
   * @return 一个将元素映射到列下标的函数
   */
  inline auto HashFunction(size_t seed) -> std::function<size_t(const KeyType &)> {
    return [seed, this](const KeyType &item) -> size_t {
      auto h1 = std::hash<KeyType>{}(item);
      auto h2 = bustub::HashUtil::CombineHashes(seed, SEED_BASE);
      return bustub::HashUtil::CombineHashes(h1, h2) % width_;
    };
  }

  /** @todo (student) 可以添加支持 count-min sketch 操作所需的数据结构 */
};

}  // namespace bustub
