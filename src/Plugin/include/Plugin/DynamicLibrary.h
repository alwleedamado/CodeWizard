#pragma once

#include <string>
#include <memory>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace CodeWizard::Plugin {

class DynamicLibrary {
public:
    explicit DynamicLibrary(const std::string& path);
    ~DynamicLibrary();

    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;

    template<typename T>
    T* getSymbol(const char* name) {
        if (!m_handle) return nullptr;
#ifdef _WIN32
        return reinterpret_cast<T*>(GetProcAddress(m_handle, name));
#else
        return reinterpret_cast<T*>(dlsym(m_handle, name));
#endif
    }

    [[nodiscard]] bool isValid() const { return m_handle != nullptr; }
    [[nodiscard]] const std::string& path() const { return m_path; }

private:
#ifdef _WIN32
    HMODULE m_handle = nullptr;
#else
    void* m_handle = nullptr;
#endif
    std::string m_path;
};

} // namespace CodeWizard::Plugin
