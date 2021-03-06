#include "talmech/utility/utility_factory.h"
#include <ros/console.h>

namespace talmech
{
namespace utility
{
const std::string UtilityFactory::delimiter_ = std::string(" ");

UtilityComponentPtr
UtilityFactory::decorate(const Expression& expression) const
{
  UtilityComponentPtr component;
  Expressions splitted_expression(split(expression));
  for (ExpressionsConstIt it(splitted_expression.begin());
       it != splitted_expression.end(); it++)
  {
    component = getComponent(*it, component);
    ROS_DEBUG_STREAM_COND(component, "Decorated " << *component << " component.");
  }
  return component;
}

Expressions
UtilityFactory::split(const Expression& expression) const
{
  Expressions splitted_expression;
  std::size_t begin(0), end(expression.find(delimiter_));
  while (begin < expression.length())
  {
    splitted_expression.push_back(expression.substr(begin, end));
    begin = begin + end + delimiter_.length();
    end = expression.find(delimiter_, end);
  }
  return splitted_expression;
}
}
}
