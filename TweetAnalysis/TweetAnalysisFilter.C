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
const char * TweetAnalysis_format_string = "%s";

void TweetAnalysis( std::vector< PacketPtr >& packets_in,
                 std::vector< PacketPtr >& packets_out,
                 std::vector< PacketPtr >& /* packets_out_reverse */,
                 void ** /* client data */,
		 PacketPtr& /* params */,
                 const TopologyLocalInfo& )
{
    fprintf(stdout, "Filter: get into tweet analysis filter\n");

    std::unordered_map<std::string, int> ans;

    for(unsigned int i = 0; i < packets_in.size( ); i++ ) {
        PacketPtr cur_packet = packets_in[i];

        char* cur_str = NULL;
        cur_packet->unpack("%s", &cur_str);
        fprintf(stdout, "Filter: receive packet %u: %s\n", i, cur_str);

        std::string ans_str(cur_str);

        size_t cur_index_1 = 0, cur_index_2 = 0;

        while (cur_index_1 < ans_str.size()) {
            cur_index_2 = ans_str.find("#", cur_index_1);
            std::string key = ans_str.substr(cur_index_1, cur_index_2 - cur_index_1);
            cur_index_1 = cur_index_2 + 1;
            cur_index_2 = ans_str.find("#", cur_index_1);
            int val = std::stoi(ans_str.substr(cur_index_1, cur_index_2 - cur_index_1), NULL);
            if (ans.count(key)) {
                ans[key] += val;
            } else {
                ans[key] = val;
            }
            cur_index_1 = cur_index_2 + 1;
            // fprintf(stdout, "key = %s, val = %d\n", key.c_str(), val);
        }

        /*
        char* key = NULL;
        int val = 0;
        cur_packet->unpack("%s %d", &key, &val);
        std::string _key(key);
        if (ans.count(_key)) {
            ans[_key] += val;
        } else {
            ans[_key] = val;
        }
        */
    }

    std::string send_str("");
    for (std::unordered_map<std::string, int>::iterator it = ans.begin(); it != ans.end(); it++) {
        send_str.append(it->first);
        send_str.append("#");
        send_str.append(std::to_string(it->second));
        send_str.append("#");
    }
    PacketPtr new_packet ( new Packet(packets_in[0]->get_StreamId(),
                                  packets_in[0]->get_Tag(), "%s", send_str.c_str() ) );
    packets_out.push_back( new_packet );
    fprintf(stdout, "Filter: send packet: %s\n", send_str.c_str());
}

} /* extern "C" */
