#include <Editor/Document.h>
#include <Platform/Path.h>
#include <catch2/catch_test_macros.hpp>

using namespace CodeWizard::Editor;

TEST_CASE("Document editing", "[editor][document]")
{
  Document doc;
  doc.insertText({ 0, 0 }, "Hello");
  REQUIRE(doc.buffer().text() == "Hello");

  doc.insertText({ 0, 5 }, " World");
  REQUIRE(doc.buffer().text() == "Hello World");

  doc.removeText({ { 0, 5 }, { 0, 6 } });// Remove space
  REQUIRE(doc.buffer().text() == "HelloWorld");
}

TEST_CASE("Document undo/redo", "[editor][document]")
{
  Document doc;
  doc.insertText({ 0, 0 }, "A");
  REQUIRE(doc.canUndo());
  doc.insertText({ 0, 1 }, "Be");
  REQUIRE(doc.canUndo());
  doc.undo();
  REQUIRE(doc.buffer().text() == "A");
  REQUIRE(doc.canRedo());
  doc.undo();
  REQUIRE(doc.buffer().text() == "");
  REQUIRE(doc.canRedo());
  doc.redo();
  REQUIRE(doc.buffer().text() == "A");
  doc.redo();
  REQUIRE(doc.buffer().text() == "ABe");
}
