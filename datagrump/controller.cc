#include <iostream>
#include <cmath>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), the_window_size(1),
  slow_start(true), acks_pending(0), rtt_timeout(250)
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp,
                                    /* in milliseconds */
				    const bool after_timeout
				    /* datagram was sent because of a timeout */ )
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */
  bool congested = (timestamp_ack_received - send_timestamp_acked) >= rtt_timeout;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock"
   << ", RTT " << (timestamp_ack_received - send_timestamp_acked) << ")"
	 << endl;
   if(congested) {
    cerr << "Congestion detected" << endl;
   }
  }
  acks_pending += 1;
  if (slow_start) {
    if (congested) {
      the_window_size = std::max(the_window_size/2, (unsigned int)2);
      slow_start = false;
    } else {
      the_window_size += 1;
    }
  } else if (acks_pending >= the_window_size) {
    acks_pending = 0;
    if(congested) {
      the_window_size = std::max(the_window_size/2, (unsigned int)2);
    } else {
      the_window_size += 1;
    }
  }

}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return rtt_timeout; /* timeout in ms */
}
