#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image &picture, unsigned max): max_(max) {
    base_image_ = new Image(picture);
}

StickerSheet::~StickerSheet() {
    for (size_t i = 0; i < stickers_.size(); i++) {
        delete stickers_.at(i).image;
    }
    delete base_image_;
}

StickerSheet::StickerSheet(const StickerSheet &other): max_(other.max_) {
    for (size_t i = 0; i < other.stickers_.size(); i++) {
        Sticker oldSticker = other.stickers_.at(i);
        stickers_.push_back({ new Image(*oldSticker.image), oldSticker.x, oldSticker.y });
    }
    base_image_ = new Image(*other.base_image_);
}

const StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
    if (&other == this)
        return *this;
    for (size_t i = 0; i < stickers_.size(); i++) {
        delete stickers_.at(i).image;
    }
    delete base_image_;

    max_ = other.max_;
    for (size_t i = 0; i < other.stickers_.size(); i++) {
        Sticker oldSticker = other.stickers_.at(i);
        stickers_.push_back({ new Image(*oldSticker.image), oldSticker.x, oldSticker.y });
    }
    base_image_ = new Image(*other.base_image_);

    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max) {
    if (max == max_) {
        return;
    } else if (max == 0) {
        while (!stickers_.empty()) {
            delete stickers_.at(stickers_.size() - 1).image;
            stickers_.pop_back();
        }
    } else if (max < stickers_.size()) {
        for (size_t i = stickers_.size() - 1; i >= max; i--) {
            delete stickers_.at(i).image;
            stickers_.pop_back();
        }
    }
    max_ = max;
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
    if (stickers_.size() == max_) {
        return -1;
    }
    Sticker newSticker = Sticker{ new Image(sticker), x, y};
    stickers_.push_back(newSticker);
    return stickers_.size() - 1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if (index >= stickers_.size()) {
        return false;
    }
    stickers_.at(index).x = x;
    stickers_.at(index).y = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    if (index < stickers_.size()) {
        delete stickers_.at(index).image;
        stickers_.erase(stickers_.begin() + index);
    }
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= stickers_.size()) {
        return NULL;
    }
    return stickers_.at(index).image;
}

Image StickerSheet::render() const {
    unsigned int max_x = base_image_->width();
    unsigned int max_y = base_image_->height();
    for (size_t i = 0; i < stickers_.size(); i++) {
        Sticker sticker = stickers_.at(i);
        max_x = std::max(max_x, sticker.x + sticker.image->width()); 
        max_y = std::max(max_y, sticker.y + sticker.image->height());
    }
    Image retval(*base_image_);
    retval.resize(max_x, max_y);

    for (size_t i = 0; i < stickers_.size(); i++) {
        Sticker sticker = stickers_.at(i);
        for (size_t x = 0; x < sticker.image->width(); x++) {
            for (size_t y = 0; y < sticker.image->height(); y++) {
                if (sticker.image->getPixel(x,y).a != 0) {
                    retval.getPixel(sticker.x + x, sticker.y + y) = sticker.image->getPixel(x,y);
                }
            }
        }
    }

    return retval;
}