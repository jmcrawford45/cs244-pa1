#include <iostream>
#include <cmath>
#include <algorithm> 

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug ), the_window_size(1.0),
  slow_start(true), dir_up(true),
  rtt_estimate(0), rtt_smoothing(0.1), velocity(1),last_window(0), rtt_min(100000), 
  outstanding_acks(0), rtt()
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return static_cast<unsigned int>(the_window_size);
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
  float sigma = 1;
  float sample_rtt = (timestamp_ack_received - send_timestamp_acked)*0.001;
  if (rtt_estimate == 0) {
    rtt_estimate = sample_rtt;
  } else {
    rtt_estimate = (1-rtt_smoothing) * rtt_estimate + rtt_smoothing * sample_rtt;
  }
  if (sample_rtt < rtt_min) {
    rtt_min = sample_rtt;
  }
  unsigned int tau = std::max((unsigned int)1,window_size()/2);
  rtt.push_back(sample_rtt);
  if (rtt.size() > tau) {
      rtt.pop_front();
  }
  float rtt_standing = *std::min_element(rtt.begin(), rtt.end());
  float lambda_estimate = the_window_size/(sigma * rtt_standing);
  float q_delay = rtt_standing-rtt_min;
  float lambda_target = 1/(sigma * q_delay);
  bool congested = lambda_estimate > lambda_target;
  if (slow_start) {
    if (congested) {
      slow_start = false;
      last_window = window_size();
      the_window_size -= velocity/the_window_size;
    } else {
      the_window_size += 1;
    }
  } else {
    outstanding_acks++;
    // if (outstanding_acks >= last_window) {
    //   if (dir_up && window_size() > last_window) {
    //     velocity *= 2;
    //   } else if (!dir_up && window_size() < last_window) {
    //     velocity *= 2;
    //   } else {
    //     velocity = 1.0;
    //   }
    //   if(window_size() <= 10) {
    //     velocity = 1;
    //   }
    //   velocity = std::min(velocity, (unsigned int)window_size()/4);
    //   last_window = window_size();
    //   outstanding_acks = 0;
    //   cerr << "v=" << velocity << endl;
    // }
    if(congested) {
      the_window_size -= velocity/(sigma * the_window_size);
    } else {
      the_window_size += velocity/(sigma * the_window_size);
    }
  }
  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock"
   << ", RTT " << sample_rtt
   << ", RTT_est " << rtt_estimate << ")"
	 << endl;
   if(congested) {
    cerr << "Congestion detected: l=" << lambda_estimate << ", l_t=" << lambda_target << endl;
   }
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return 5000 * rtt_estimate; /* timeout in ms */
}
