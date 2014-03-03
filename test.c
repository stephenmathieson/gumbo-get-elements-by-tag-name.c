
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gumbo-parser/gumbo.h"
#include "gumbo-get-element-by-id/get-element-by-id.h"
#include "get-elements-by-tag-name.h"

#define EQUALS(a, b) ({                               \
  if (0 != strcmp(a, b))  {                           \
    fprintf(stderr                                    \
      , "Assertion error: \"%s\" == \"%s\" (%s:%d)\n" \
      , a                                             \
      , b                                             \
      , __FILE__                                      \
      , __LINE__);                                    \
    abort();                                          \
  }                                                   \
})

#define TEST(name) \
  static void test_##name(void)

#define RUN_TEST(test) \
  test_##test(); \
  puts("    \e[92m✓ \e[90m" #test "\e[0m");

// read a file
static char *
read_file(const char *file) {
  FILE *fp = NULL;
  char *buffer = NULL;
  long size = 0;

  fp = fopen(file, "r");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);

  fseek(fp, 0, SEEK_SET);

  buffer = (char *) calloc(size + 1, sizeof(char));
  assert(buffer);
  *buffer = '\0';
  fread(buffer, sizeof(char), size, fp);
  fclose(fp);

  return buffer;
}

TEST(simple_uppercase_tag) {
  char *html = read_file("./tests/simple.html");
  assert(html);
  GumboOutput *output = gumbo_parse(html);
  free(html);
  assert(output);

  list_t *elements = gumbo_get_elements_by_tag_name("H1", output->root);
  assert(elements && 1 == (int) elements->len);
  list_destroy(elements);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(simple_find_h2s) {
  char *html = read_file("./tests/simple.html");
  assert(html);
  GumboOutput *output = gumbo_parse(html);
  free(html);
  assert(output);

  list_t *elements = gumbo_get_elements_by_tag_name("h2", output->root);
  assert(elements && 2 == (int) elements->len);
  list_destroy(elements);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(unsupported_tag) {
  char *html = read_file("./tests/simple.html");
  assert(html);
  GumboOutput *output = gumbo_parse(html);
  free(html);
  assert(output);

  assert(NULL == gumbo_get_elements_by_tag_name("foo", output->root));
  assert(NULL == gumbo_get_elements_by_tag_name("bar", output->root));
  assert(NULL == gumbo_get_elements_by_tag_name("baz", output->root));
  assert(NULL == gumbo_get_elements_by_tag_name("", output->root));
  assert(NULL == gumbo_get_elements_by_tag_name(NULL, output->root));
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(middle_of_document) {
  char *html = read_file("./tests/simple.html");
  assert(html);
  GumboOutput *output = gumbo_parse(html);
  free(html);
  assert(output);

  GumboNode *middle =
    (GumboNode *) gumbo_get_element_by_id("middle", output->root);
  assert(middle);

  list_t *items = gumbo_get_elements_by_tag_name("li", middle);
  assert(items && 5 == (int) items->len);
  list_destroy(items);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

int
main(void) {
  clock_t start = clock();

  printf("\n  \e[36m%s\e[0m\n", "gumbo_get_elements_by_tag_name");

  RUN_TEST(simple_uppercase_tag);
  RUN_TEST(simple_find_h2s);
  RUN_TEST(unsupported_tag);
  RUN_TEST(middle_of_document);

  printf("\n");
  printf("  \e[90mcompleted in \e[32m%.5fs\e[0m\n"
    , (float) (clock() - start) / CLOCKS_PER_SEC);
  printf("\n");
}
