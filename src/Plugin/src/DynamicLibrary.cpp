#include "Plugin/DynamicLibrary.h"
#include <stdexcept>

using namespace CodeWizard::Plugin;

DynamicLibrary::DynamicLibrary(const std::string& path) : m_path(path) {
#ifdef _WIN32
    m_handle = LoadLibraryA(path.c_str());
    if (!m_handle) {
        throw std::runtime_error("Failed to load library: " + path);
    }
#else
    m_handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!m_handle) {
        throw std::runtime_error("Failed to load library: " + std::string(dlerror()));
    }
#endif
}

DynamicLibrary::~DynamicLibrary() {
    if (m_handle) {
#ifdef _WIN32
        FreeLibrary(m_handle);
#else
        dlclose(m_handle);
#endif
    }
}

DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
    : m_handle(other.m_handle), m_path(std::move(other.m_path)) {
    other.m_handle = nullptr;
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
    if (this != &other) {
        if (m_handle) {
#ifdef _WIN32
            FreeLibrary(m_handle);
#else
            dlclose(m_handle);
#endif
        }
        m_handle = other.m_handle;
        m_path = std::move(other.m_path);
        other.m_handle = nullptr;
    }
    return *this;
}
