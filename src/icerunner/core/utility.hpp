#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <algorithm>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ice
{

namespace core
{

namespace utility
{

template <typename T_>
void DeleteAndNull(T_*& p)
{
    delete p;
    p = nullptr;
}

template <typename Container_>
void DeleteAndNull(Container_& container)
{
    std::for_each(container.begin(), container.end(), [](typename Container_::value_type& p) {
        DeleteAndNull(p);
    });
}

template <typename Container_>
void DeleteNullAndClear(Container_& container)
{
    std::for_each(container.begin(), container.end(), [](typename Container_::value_type& p) {
        DeleteAndNull(p);
    });
    container.clear();
}

template <typename Container_>
bool AnyAreNull(Container_& container)
{
    return std::any_of(container.begin(), container.end(), [](void* p) {
        return p == nullptr;
    });
}

//! Calls a function object on the file names of each file in a directory.
//!
//! \tparam FileHandler_ The type of the file handling function object (taking in const char*).
//! \param dir The directory to traverse.
//! \note dir MUST end in a backslash.
//!
template <typename FileHandler_>
void ForEachFile(const std::string& dir, FileHandler_ handler)
{
    std::string dirGlob = dir + '*';

    WIN32_FIND_DATAA searchData = {};
    HANDLE handle = FindFirstFileA(dirGlob.c_str(), &searchData);
    while (handle != INVALID_HANDLE_VALUE)
    {
        if (!(searchData.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN)))
            handler(searchData.cFileName);
        if (FindNextFileA(handle, &searchData) == FALSE) break;
    }
    FindClose(handle);
}

} // namespace utility

} // namespace core

} // namespace ice

#endif // UTILITY_HPP
