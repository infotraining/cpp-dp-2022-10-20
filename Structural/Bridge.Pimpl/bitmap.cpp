#include "bitmap.hpp"
#include <algorithm>
#include <array>

using namespace std;

struct Bitmap::Impl
{
    std::vector<char> image_;
};

Bitmap::Bitmap(size_t size, char fill_char) : pimpl_{std::make_unique<Impl>()}
{
    pimpl_->image_ = std::vector<char>(size, fill_char);
    fill_n(pimpl_->image_.begin(), size, fill_char);
}

Bitmap::~Bitmap() = default;

void Bitmap::draw()
{
    cout << "Image: ";
    for (size_t i = 0; i < pimpl_->image_.size(); ++i)
        cout << pimpl_->image_[i];
    cout << endl;
}
