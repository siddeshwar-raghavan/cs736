/****************************************************************************
 * Copyright � 2003-2015 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include <unordered_map>
#include <cctype>
#include <fstream>

#include "mrnet/MRNet.h"
#include "TweetAnalysis.h"

using namespace MRN;

int tweetCountFile(const char* file_path, const std::string target) {
    int ans = 0;

    // filestream variable file
    std::ifstream file;
    std::string line;

    // opening file
    file.open(file_path);

    // extracting words from the file
    while (std::getline(file, line))
    {
        // check if line contains target
        if (line.find(target) != std::string::npos) {
            ans++;
        }
    }
    file.close();
    return ans;
}

int main(int argc, char **argv)
{
    Stream * stream = NULL;
    PacketPtr p;
    int rc, tag=0, send_val = 100;
    char* recv_val = NULL;
    char buffer[100];
    const char* file_path_pattern = "/home/mazijun/Documents/readings/CS736/mrnet-3093918/Examples/TweetAnalysis/test-%d.txt";

    Network * net = Network::CreateNetworkBE( argc, argv );
    sprintf(buffer, file_path_pattern, net->get_LocalRank());

    do {
        rc = net->recv(&tag, p, &stream);
        if( rc == -1 ) {
            fprintf( stderr, "BE: Network::recv() failure\n" );
            break;
        }
        else if( rc == 0 ) {
            // a stream was closed
            continue;
        }

        int tweet_count = 0;

        switch(tag) {

        case PROT_SUM:
            p->unpack( "%s", &recv_val);
            fprintf(stdout, "BE: receive word %s.\n", recv_val);

            tweet_count = tweetCountFile(buffer, std::string(recv_val));

            if( stream->send(tag, "%d", tweet_count) == -1 ) {
                fprintf( stderr, "BE: stream::send(%%d) failure in PROT_SUM\n" );
                tag = PROT_EXIT;
                break;
            }

            if( stream->flush() == -1 ) {
                fprintf( stderr, "BE: stream::flush() failure in PROT_SUM\n" );
                break;
            }
            fflush(stdout);
            sleep(2); // stagger sends
            break;

        case PROT_EXIT:
            if( stream->send(tag, "%d", 0) == -1 ) {
                fprintf( stderr, "BE: stream::send(%%s) failure in PROT_EXIT\n" );
                break;
            }
            if( stream->flush( ) == -1 ) {
                fprintf( stderr, "BE: stream::flush() failure in PROT_EXIT\n" );
            }
            break;

        default:
            fprintf( stderr, "BE: Unknown Protocol: %d\n", tag );
            tag = PROT_EXIT;
            break;
        }

        fflush(stderr);

    } while( tag != PROT_EXIT );

    if( stream != NULL ) {
        while( ! stream->is_Closed() )
            sleep(1);

        delete stream;
    }

    // FE delete of the net will cause us to exit, wait for it
    net->waitfor_ShutDown();
    delete net;

    return 0;
}
