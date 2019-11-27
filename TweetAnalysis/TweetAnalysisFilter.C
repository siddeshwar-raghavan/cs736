/****************************************************************************
 * Copyright � 2003-2015 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include <vector>
#include <unordered_map>

#include "mrnet/Packet.h"
#include "mrnet/NetworkTopology.h"

using namespace MRN;

extern "C" {
const char * TweetAnalysis_format_string = "%d";

void TweetAnalysis( std::vector< PacketPtr >& packets_in,
                 std::vector< PacketPtr >& packets_out,
                 std::vector< PacketPtr >& /* packets_out_reverse */,
                 void ** /* client data */,
                 PacketPtr& /* params */,
                 const TopologyLocalInfo& )
{
    fprintf(stdout, "Filter: get into tweet analysis filter\n");

    int sum = 0;

    for( unsigned int i = 0; i < packets_in.size( ); i++ ) {
        PacketPtr cur_packet = packets_in[i];
        int val;
        cur_packet->unpack("%d", &val);
        sum += val;
    }

    PacketPtr new_packet ( new Packet(packets_in[0]->get_StreamId(),
                                      packets_in[0]->get_Tag(), "%d", sum ) );
    packets_out.push_back( new_packet );
    fprintf(stdout, "Filter: send packet: %d\n", sum);
}

} /* extern "C" */
