#include "talmech/auction/bidding/awaiting_auction_close.h"
#include "talmech/auction/bidding/bidding_controller.h"
#include <talmech_msgs/Bid.h>

namespace talmech
{
namespace auction
{
namespace bidding
{
AwaitingAuctionClose::AwaitingAuctionClose(
    const BiddingControllerPtr& controller, const ros::Duration& tolerance)
    : BiddingState::BiddingState(controller, states::AwaitingAuctionClose),
      selected_(false), tolerance_(tolerance)
{
}

bool AwaitingAuctionClose::preProcess()
{
  if (!publisher_)
  {
    throw Exception("The submission publisher has not been registered yet.");
  }
  selected_ = false;
  publisher_->publish(bid_->toMsg());
  return MachineState::preProcess();
}

bool AwaitingAuctionClose::process()
{
  return !hasClosed() && !hasExpired() ? false : MachineState::process();
}

bool AwaitingAuctionClose::hasExpired() const
{
  return close_timestamp_.isZero() &&
         ros::Time::now() - auction_->getStartTimestamp() >
             auction_->getDuration() + tolerance_;
}

int AwaitingAuctionClose::getNext() const
{
  return selected_ ? states::AwaitingContractRenewal
                   : states::AwaitingBiddingDisposal;
}

void AwaitingAuctionClose::closeCallback(const talmech_msgs::Acknowledgment &msg)
{
  if (msg.auction != auction_->getId())
  {
    return;
  }
  close_timestamp_ = ros::Time::now();
  selected_ = msg.bidder == bid_->getBidder();
}
}
}
}
