// include/Packaging/Packager.h
#pragma once

#include <Core/Types.h>
#include <Platform/Path.h>

namespace CodeWizard::Packaging {

class Packager {
public:
    static void createAppImage(const Platform::Path& sourceDir, const Platform::Path& outputPath);
    static void createDebPackage(const Platform::Path& sourceDir, const Platform::Path& outputPath);
};

} // namespace CodeWizard::Packaging
