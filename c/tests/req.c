#include "test.h"
#include "../both/req.h"

void test_req() {
  char s[MAX_LINE_LEN];
  announce("its_fine_with_extras", 2 == req_parse("<2,4> hello"));

  announce("errors_on_empty", -1 == req_parse(""));
  announce("errors_on_no_num", -1 == req_parse("<,>"));
  announce("errors_on_letter", -1 == req_parse("<2a,4>"));
  announce("parses_one_digit", 2 == req_parse("<2,4>"));
  announce("takes_many_digits", 7325 == req_parse("<7325,4444>"));
  announce("no_error_when_fine", -1 != req_parse("<7325,4444>"));
  announce("errors_on_long", -1 == req_parse(
        "<"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"

        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"

        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"

        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"

        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"
        "22222222222222222222222222222222222222222222222222"

        "22222222222222222222222222222222222222222222222222"
        ",4>"
        ));

  announce("finding_line_0", 0 == strcmp("a", isolate_test(s, "a\0", 0, 2)));
  announce("finding_line_1", 0 == strcmp("b", isolate_test(s, "a\nb\0", 1, 2)));
  announce("not_finding_line_1", NULL == isolate_test(s, "a\0b\0", 1, 2));
  announce("finding_empty_line", 0 == strcmp("", isolate_test(s, "\0", 0, 2)));
  announce("finding_multi_char", 0 == strcmp("baba", isolate_test(s, "a\nhi \nbaba\n dude\0", 2, 5)));
}