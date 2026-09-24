#include <cstring>
#include <string.h>
#include <cstddef>

namespace replusplus {
struct ControlBlock {
    std::size_t ref_ct_ {1};
};

// TODO: Account for resize
class COWString {
public:
    COWString() {}
    ~COWString() {
        if (ctrl_blk_) {
            if (ctrl_blk_->ref_ct_ == 1) {
                // We are the last owner, destroy
                delete ctrl_blk_;
                delete[] cstr_;
            }
            else {
                // We are a viewer who left
                ctrl_blk_->ref_ct_--;
            }
        }
    }
    explicit COWString(const char* c) {
        size_ = strlen(c);
        cstr_ = new char[size_ + 1];
        cstr_[size_] = '\0';

        strncpy(cstr_, c, size_);

        ctrl_blk_ = new ControlBlock {};
    }

    COWString(const COWString& other) {
        // Default to shallow copy
        cstr_ = other.cstr_;
        size_ = other.size_;
        ctrl_blk_ = other.ctrl_blk_;
        ctrl_blk_->ref_ct_++;
    }

    char operator[](std::size_t idx) const {
        return cstr_[idx];
    }
    char& operator[](std::size_t idx) {
        // On edit generate new string and work on that
        // UNLESS we are the only unique viewer
        if (is_only_viewer()) {
            return cstr_[idx];
        }

        // If multiple viewers exist, we should create our own string here
        ctrl_blk_->ref_ct_--;

        char* new_cstr_ = new char[size_ + 1];
        new_cstr_[size_] = '\0';

        strncpy(new_cstr_, cstr_, size_);

        ctrl_blk_ = new ControlBlock {};

        return cstr_[idx];
    }

    const char* c_str() const { return cstr_; }
    std::size_t size() const { return size_; }

private:
    char* cstr_ {};
    std::size_t size_ {};

    bool is_only_viewer() const { return ctrl_blk_ && ctrl_blk_->ref_ct_ == 1; };

    ControlBlock* ctrl_blk_ {};
};
} // namespace replusplus