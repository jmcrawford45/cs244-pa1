#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>
#include <queue> 

/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */
  float the_window_size;
  bool slow_start;
  bool dir_up;
  float rtt_estimate;
  float rtt_smoothing;
  unsigned int velocity;
  unsigned int last_window;
  float rtt_min;
  unsigned int outstanding_acks;
  std::deque<float> rtt;

public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug );

  /* Get current window size, in datagrams */
  unsigned int window_size();

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp,
			  const bool after_timeout );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received );

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms();
};

#endif
