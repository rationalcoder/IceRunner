#ifndef COMPTRHACK_HPP
#define COMPTRHACK_HPP
#include <d3d11.h>

//! ComPtr workaround until we get atlbase.h from wine (assuming that it does what we want)
template <typename COMObj_>
class ComPtr
{
public:
    ComPtr()
        : obj_(nullptr)
    {}

    ~ComPtr()
    {
        if (obj_)
        {
            obj_->Release();
            obj_ = nullptr;
        }
    }

    ComPtr(ComPtr& copy)
    {
        COMObj_* obj = copy.obj_;
        if (obj) obj->AddRef();
        obj_ = obj;
    }

    ComPtr& operator=(ComPtr& rhs)
    {
        if (obj_) obj_->Release();
        obj_->AddRef();
        obj_ = rhs.obj_;
        return *this;
    }

    COMObj_** operator&()
    {
        if (obj_) obj_->Release();
        return &obj_;
    }

    //! For functions that add a reference.
    COMObj_** AddressOf() { return &obj_; }

    void Release() { this->~ComPtr(); }

    COMObj_* operator->() { return obj_; }
    operator COMObj_*() { return obj_; }

private:
    COMObj_* obj_;
};

#endif // COMPTRHACK_HPP
