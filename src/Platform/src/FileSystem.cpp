// src/FileSystem.cpp
#include "Platform/FileSystem.h"
#include <Core/Logger.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <system_error>

namespace fs = std::filesystem;
using namespace CodeWizard::Core;

namespace CodeWizard::Platform {

namespace {
  ErrorCode mapErrorCode(const std::error_code &ec)
  {
    if (!ec) return ErrorCode::Success;
    switch (ec.value()) {
    case ENOENT:
      return ErrorCode::FileNotFound;
    case EACCES:
    case EPERM:
      return ErrorCode::PermissionDenied;
    case EINVAL:
      return ErrorCode::InvalidArgument;
    default:
      return ErrorCode::IoError;
    }
  }
}// namespace

Result<std::string> readFile(const Path &path, ReadOption option)
{
  if (!path.exists()) { return failure<std::string>(ErrorCode::FileNotFound, "File does not exist: " + path.native()); }

  std::ios_base::openmode mode = std::ios::in;
  if (option == ReadOption::Binary) { mode |= std::ios::binary; }

  std::ifstream file(path.native(), mode);
  if (!file.is_open()) { return failure<std::string>(ErrorCode::IoError, "Cannot open file: " + path.native()); }

  std::string content;
  try {
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
  } catch (const std::exception &e) {
    return failure<std::string>(ErrorCode::IoError, std::string("Read error: ") + e.what());
  }

  return success<std::string>(std::move(content));
}

Result<void> writeFile(const Path &path, std::string_view content, WriteOption option)
{
  std::ios_base::openmode mode = std::ios::out;
  if (option == WriteOption::Binary) { mode |= std::ios::binary; }
  if (option == WriteOption::Append) { mode |= std::ios::app; }

  std::ofstream file(path.native(), mode);
  if (!file.is_open()) { return failure(ErrorCode::IoError, "Cannot create file: " + path.native()); }

  try {
    file.write(content.data(), content.size());
  } catch (const std::exception &e) {
    return failure(ErrorCode::IoError, std::string("Write error: ") + e.what());
  }

  return success();
}

Result<std::vector<FileInfo>> listDirectory(const Path &dir)
{
  if (!dir.exists()) {
    return failure<std::vector<FileInfo>>(ErrorCode::FileNotFound, "Directory does not exist: " + dir.native());
  }
  if (!dir.isDirectory()) {
    return failure<std::vector<FileInfo>>(ErrorCode::InvalidArgument, "Not a directory: " + dir.native());
  }

  std::vector<FileInfo> result;
  std::error_code ec;
  for (const auto &entry : fs::directory_iterator(dir.native(), ec)) {
    if (ec) { return failure<std::vector<FileInfo>>(mapErrorCode(ec), "Directory iteration failed"); }

    FileInfo info;
    info.path = Path(entry.path().string());
    info.isDirectory = entry.is_directory(ec);
    info.isFile = entry.is_regular_file(ec);
    info.size = entry.file_size(ec);
    auto ftime = entry.last_write_time(ec);
    info.lastModified = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
    result.push_back(std::move(info));
  }

  return success<std::vector<FileInfo>>(result);
}

Result<void> createDirectory(const Path &dir, bool recursive)
{
  std::error_code ec;
  if (recursive) {
    fs::create_directories(dir.native(), ec);
  } else {
    fs::create_directory(dir.native(), ec);
  }
  if (ec) { return failure(mapErrorCode(ec), "Failed to create directory: " + dir.native()); }
  return success();
}

Result<void> remove(const Path &path, bool recursive)
{
  if (!path.exists()) { return failure(ErrorCode::FileNotFound, "Path does not exist: " + path.native()); }

  std::error_code ec;
  if (recursive && path.isDirectory()) {
    fs::remove_all(path.native(), ec);
  } else {
    fs::remove(path.native(), ec);
  }
  if (ec) { return failure(mapErrorCode(ec), "Failed to remove: " + path.native()); }
  return success();
}

}// namespace CodeWizard::Platform
