#include "oneflow/core/common/flat_msg_view.h"
#include "oneflow/core/common/util.h"

namespace oneflow {

namespace test {

namespace {

// clang-format off
FLAT_MSG_BEGIN(VariantFoo);
  FLAT_MSG_DEFINE_STRICT_ONEOF(_,
    FLAT_MSG_ONEOF_FIELD(int8_t, int8_value)
    FLAT_MSG_ONEOF_FIELD(int16_t, int16_value)
    FLAT_MSG_ONEOF_FIELD(int32_t, int32_value)
    FLAT_MSG_ONEOF_FIELD(float, float_value));
FLAT_MSG_END(VariantFoo);
// clang-format on

// clang-format off
FLAT_MSG_BEGIN(VariantList);
  FLAT_MSG_DEFINE_REPEATED(VariantFoo, foo, 16);
FLAT_MSG_END(VariantList);
// clang-format on

// clang-format off
FLAT_MSG_VIEW_BEGIN(ViewFoo);
  FLAT_MSG_VIEW_DEFINE_PATTERN(int32_t, int32_value);
  FLAT_MSG_VIEW_DEFINE_PATTERN(int16_t, int16_value);
  FLAT_MSG_VIEW_DEFINE_PATTERN(float, float_value);
FLAT_MSG_VIEW_END(ViewFoo);
// clang-format on

TEST(FlatMsgView, match_success) {
  FlatMsg<VariantList> variant_list;
  variant_list.Mutable()->mutable_foo()->Add()->set_int32_value(30);
  variant_list.Mutable()->mutable_foo()->Add()->set_int16_value(40);
  variant_list.Mutable()->mutable_foo()->Add()->set_float_value(50.0);
  FlatMsgView<ViewFoo> view;
  ASSERT_TRUE(view->template Match(variant_list.Mutable()->mutable_foo()));
  ASSERT_EQ(view->int32_value(), 30);
  ASSERT_EQ(view->int16_value(), 40);
  ASSERT_EQ(view->float_value(), 50.0);
}

TEST(FlatMsgView, match_failed) {
  FlatMsg<VariantList> variant_list;
  variant_list.Mutable()->mutable_foo()->Add()->set_int16_value(40);
  variant_list.Mutable()->mutable_foo()->Add()->set_int32_value(30);
  variant_list.Mutable()->mutable_foo()->Add()->set_float_value(50.0);
  FlatMsgView<ViewFoo> view;
  ASSERT_TRUE(!view->template Match(variant_list.Mutable()->mutable_foo()));
}

TEST(FlatMsgView, match_success_vector) {
  std::vector<FlatMsg<VariantFoo>> variant_list(3);
  variant_list.at(0)->set_int32_value(30);
  variant_list.at(1)->set_int16_value(40);
  variant_list.at(2)->set_float_value(50.0);
  FlatMsgView<ViewFoo> view;
  ASSERT_TRUE(view->template Match(&variant_list));
  ASSERT_EQ(view->int32_value(), 30);
  ASSERT_EQ(view->int16_value(), 40);
  ASSERT_EQ(view->float_value(), 50.0);
}

TEST(FlatMsgView, match_failed_vector) {
  std::vector<FlatMsg<VariantFoo>> variant_list(3);
  variant_list.at(0)->set_int16_value(40);
  variant_list.at(1)->set_int32_value(30);
  variant_list.at(2)->set_float_value(50.0);
  FlatMsgView<ViewFoo> view;
  ASSERT_TRUE(!view->template Match(&variant_list));
}

TEST(FlatMsgView, init) {
  FlatMsg<VariantList> variant_list;
  {
    FlatMsgView<ViewFoo> mut_view(variant_list.Mutable()->mutable_foo());
    mut_view->set_int32_value(30);
    mut_view->set_int16_value(40);
    mut_view->set_float_value(50.0);
  }
  {
    FlatMsgView<ViewFoo> view;
    ASSERT_TRUE(view->Match(variant_list.Mutable()->mutable_foo()));
    ASSERT_EQ(view->int32_value(), 30);
    ASSERT_EQ(view->int16_value(), 40);
    ASSERT_EQ(view->float_value(), 50.0);
  }
}

TEST(FlatMsgView, init_vector) {
  std::vector<FlatMsg<VariantFoo>> variant_list;
  {
    FlatMsgView<ViewFoo> mut_view(&variant_list);
    mut_view->set_int32_value(30);
    mut_view->set_int16_value(40);
    mut_view->set_float_value(50.0);
  }
  {
    FlatMsgView<ViewFoo> view;
    ASSERT_TRUE(view->Match(&variant_list));
    ASSERT_EQ(view->int32_value(), 30);
    ASSERT_EQ(view->int16_value(), 40);
    ASSERT_EQ(view->float_value(), 50.0);
  }
}

}  // namespace

}  // namespace test

}  // namespace oneflow
