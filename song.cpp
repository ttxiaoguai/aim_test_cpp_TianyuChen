#include "Song.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

// ================================================================
// 工具函数（必须在匿名命名空间外，否则测试中的符号会不匹配）
// ================================================================
std::string trim_copy(const std::string &s) {
    size_t start = 0, end = s.size();
    while (start < end && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

std::string to_lower_copy(const std::string &s) {
    std::string t = s;
    std::transform(t.begin(), t.end(), t.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return t;
}

std::string join_tags(const std::vector<std::string> &tags) {
    if (tags.empty()) return "";
    std::string out = tags[0];
    for (size_t i = 1; i < tags.size(); ++i) {
        out += ", " + tags[i];
    }
    return out;
}

// ================================================================
// 静态成员
// ================================================================
int Song::next_id_ = 1;

// ================================================================
// 构造函数
// ================================================================
Song::Song(const std::string &title,
           const std::string &artist,
           int duration_sec,
           int rating)
    // 关键修正 1：使用初始化列表确保所有成员有初始值，特别是 valid_ 默认是 false
    : id_(0), duration_sec_(0), rating_(0), valid_(false)
{
    std::string t = trim_copy(title);
    std::string a = trim_copy(artist);

    // 验证逻辑
    if (t.empty()) {
        std::cout << "[错误] 标题不能为空\n";
        return;
    }
    if (a.empty()) {
        std::cout << "[错误] 艺人不能为空\n";
        return;
    }
    if (duration_sec <= 0) {
        std::cout << "[错误] 时长必须为正整数（秒）\n";
        return;
    }
    if (rating < 1 || rating > 5) {
        std::cout << "[错误] 评分必须在 1...5 之间\n";
        return;
    }

    // 验证通过
    id_ = next_id_++;
    title_ = t;
    artist_ = a;
    duration_sec_ = duration_sec;
    rating_ = rating;
    valid_ = true;
}

// ================================================================
// Setter：标题
// ================================================================
bool Song::set_title(const std::string &t) {
    std::string s = trim_copy(t);
    if (s.empty()) {
        std::cout << "[提示] 标题不能为空，已忽略本次修改\n";
        return false;
    }
    title_ = s;
    return true;
}

// ================================================================
// Setter：艺人
// ================================================================
bool Song::set_artist(const std::string &a) {
    std::string s = trim_copy(a);
    if (s.empty()) {
        std::cout << "[提示] 艺人不能为空，已忽略本次修改\n";
        return false;
    }
    artist_ = s;
    return true;
}

// ================================================================
// Setter：时长
// ================================================================
bool Song::set_duration(int sec) {
    if (sec <= 0) {
        std::cout << "[提示] 时长需为正整数，已忽略本次修改\n";
        return false;
    }
    duration_sec_ = sec;
    return true;
}

// ================================================================
// Setter：评分
// ================================================================
bool Song::set_rating(int r) {
    if (r < 1 || r > 5) {
        std::cout << "[提示] 评分需在 1..5，已忽略本次修改\n";
        return false;
    }
    rating_ = r;
    return true;
}

// ================================================================
// 添加标签
// ================================================================
bool Song::add_tag(const std::string &tag) {
    std::string raw = trim_copy(tag);
    if (raw.empty()) {
        std::cout << "[提示] 空标签已忽略\n";
        return false;
    }

    std::string low = to_lower_copy(raw);
    for (const auto &x : tags_) {
        if (to_lower_copy(x) == low) {
            std::cout << "[提示] 标签已存在（忽略大小写）\n";
            return false;
        }
    }

    tags_.push_back(raw);
    return true;
}

// ================================================================
// 移除标签
// ================================================================
bool Song::remove_tag(const std::string &tag) {
    std::string low = to_lower_copy(trim_copy(tag));

    for (size_t i = 0; i < tags_.size(); ++i) {
        if (to_lower_copy(tags_[i]) == low) {
            tags_.erase(tags_.begin() + i);
            return true;
        }
    }

    std::cout << "[提示] 未找到该标签\n";
    return false;
}

// ================================================================
// 匹配关键词
// ================================================================
bool Song::matches_keyword(const std::string &kw) const {
    std::string key = to_lower_copy(trim_copy(kw));
    if (key.empty()) return false;

    // 搜索标题和艺人
    if (to_lower_copy(title_).find(key) != std::string::npos) return true;
    if (to_lower_copy(artist_).find(key) != std::string::npos) return true;

    // 搜索标签
    for (const auto &x : tags_) {
        if (to_lower_copy(x).find(key) != std::string::npos) return true;
    }

    return false;
}

// ================================================================
// operator<<
// ================================================================
std::ostream &operator<<(std::ostream &os, const Song &s) {
    os << "[#" << s.id_ << "] "
       << s.artist_ << " - " << s.title_
       << " (" << s.duration_sec_ << "s) "
       << std::string(s.rating_, '*');

    if (!s.tags_.empty()) {
        os << " [tags: " << join_tags(s.tags_) << "]";
    }

    return os;
}

// ================================================================
// operator<
// ================================================================
// 关键修正 2：改为按 Title 升序，然后按 Artist 升序
bool operator<(const Song &a, const Song &b) {
    // 1. 主要排序：按 Title 升序
    if (a.title_ < b.title_) return true;
    if (a.title_ > b.title_) return false;

    // 2. 次要排序：如果 Title 相同，按 Artist 升序
    if (a.artist_ < b.artist_) return true;
    if (a.artist_ > b.artist_) return false;

    // 3. 最终排序（如果 Title 和 Artist 都相同）：按 ID 升序（保证稳定排序）
    return a.id_ < b.id_;
}