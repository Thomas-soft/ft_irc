#include "../includes/Channel.hpp"

Channel::Channel() : _name(""), _key(""), _topic_set(false), _topic(""), _invite_only(false), _limit(0)
{
}

Channel::~Channel()
{
}
