// :bustub-keep-private:
//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// arc_replacer.cpp
//
// Identification: src/buffer/arc_replacer.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/arc_replacer.h"
#include <optional>
#include "common/config.h"

namespace bustub {

/**
 *
 * TODO(P1): 补充实现
 *
 * @brief 创建一个新的 ArcReplacer，所有链表初始为空，目标大小初始为 0
 * @param num_frames ArcReplacer 需要缓存的最大帧数
 */
ArcReplacer::ArcReplacer(size_t num_frames) : curr_size_(0), mru_target_size_(0), replacer_size_(num_frames) {

}

/**
 * TODO(P1): 补充实现
 *
 * @brief 按照题目说明执行 Replace 操作，
 * 根据平衡策略从 mfu_ 或 mru_ 中驱逐一个元素到对应的幽灵链表。
 *
 * 如果你想参考原始 ARC 论文，请注意我们的实现做了两处调整：
 * 1. 当 mru_ 的大小等于目标大小时，决定从哪个链表驱逐时，
 * 不再像论文里那样检查最近一次访问情况。
 * 这是可以接受的，因为论文中说明这里的选择本身就是任意的。
 * 2. 不可驱逐的条目会被跳过。如果目标一侧
 * （mru_ / mfu_）的所有条目都被固定，我们会改为尝试从另一侧
 * （mfu_ / mru_）选择牺牲项，并将其移动到对应的幽灵链表
 * （mfu_ghost_ / mru_ghost_）。
 *
 * @return 被驱逐帧的 frame id；如果无法驱逐则返回 std::nullopt
 */
auto ArcReplacer::Evict() -> std::optional<frame_id_t> 
{
    std::scoped_lock lock(latch_);
    auto find_evict=[&](std::list<frame_id_t> &src,std::list<page_id_t> &ghost,ArcStatus ghost_status)->std::optional<frame_id_t>
    {
        for(auto it=src.begin();it!=src.end();it++)
        {
            frame_id_t fid=*it;
            auto alive_it=alive_map_.find(fid);
        }
    };
}

/**
 * TODO(P1): 补充实现
 *
 * @brief 记录一次对帧的访问，并相应更新 ARC 的维护状态。
 * 如果被访问的页面已经存在于某个链表中，就把它移动到 mfu_ 的表头；
 * 如果不存在于任何链表中，就把它放到 mru_ 的表头。
 *
 * 这里执行原论文中除 REPLACE 之外的所有操作，REPLACE 由 `Evict()` 处理。
 *
 * 需要考虑并分别处理以下四种情况：
 * 1. 访问命中 mru_ 或 mfu_
 * 2/3. 访问命中 mru_ghost_ / mfu_ghost_
 * 4. 访问没有命中任何链表
 *
 * 这个函数会完成对四个链表的所有必要调整，
 * 以便 `Evict()` 只需要找到受害页并将其驱逐到幽灵链表中。
 *
 * 注意：仍然存活的页面使用 frame_id 作为标识，
 * 幽灵页面使用 page_id 作为标识，因为页面被驱逐后 page_id 才是其唯一标识。
 * 对于存活页面来说，使用 page_id 其实也等价，因为二者是一一对应的，
 * 但使用 frame_id 会更直观一些。
 *
 * @param frame_id 收到新一次访问的帧 id
 * @param page_id 映射到该帧的页面 id
 * @param access_type 本次访问的类型。这个参数只在排行榜测试中会用到。
 */
void ArcReplacer::RecordAccess(frame_id_t frame_id, page_id_t page_id, [[maybe_unused]] AccessType access_type) {}

/**
 * TODO(P1): 补充实现
 *
 * @brief 切换某个帧是否可驱逐。这个函数同时也负责维护 replacer 的大小。
 * 注意，这里的大小等于当前可驱逐条目的数量。
 *
 * 如果某个帧原本可驱逐，现在被设置为不可驱逐，那么大小应当减一。
 * 如果某个帧原本不可驱逐，现在被设置为可驱逐，那么大小应当加一。
 *
 * 如果 frame id 非法，应抛出异常或直接终止进程。
 *
 * 其他情况下，这个函数应当直接结束且不做任何修改。
 *
 * @param frame_id 要修改“是否可驱逐”状态的帧 id
 * @param set_evictable 指定该帧是否应设为可驱逐
 */
void ArcReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {}

/**
 * TODO(P1): 补充实现
 *
 * @brief 从 replacer 中移除一个可驱逐帧。
 * 如果移除成功，这个函数还应当将 replacer 的大小减一。
 *
 * 注意，这和驱逐帧不同。驱逐操作总是移除由 ARC 算法选出的那个帧。
 *
 * 如果对一个不可驱逐帧调用 Remove，应抛出异常或直接终止进程。
 *
 * 如果找不到指定的帧，直接返回即可。
 *
 * @param frame_id 要移除的帧 id
 */
void ArcReplacer::Remove(frame_id_t frame_id) {}

/**
 * TODO(P1): 补充实现
 *
 * @brief 返回 replacer 的大小，也就是当前可驱逐帧的数量。
 *
 * @return size_t
 */
auto ArcReplacer::Size() -> size_t { return 0; }

}  // namespace bustub
