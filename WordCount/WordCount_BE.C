/****************************************************************************
 * Copyright � 2003-2015 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include <unordered_map>
#include <cctype>
#include <fstream>

#include "mrnet/MRNet.h"
#include "WordCount.h"

using namespace MRN;

std::unordered_map<std::string, int> wordCountFile(const char* file_path) {
    std::unordered_map<std::string, int> m;

    // filestream variable file
    std::ifstream file;
    std::string word;

    // opening file
    file.open(file_path);

    // extracting words from the file
    while (file >> word)
    {
        // displaying content
        int start_index = 0, end_index = (int)word.size() - 1;
        while (ispunct(word[start_index])) {
            start_index++;
        }
        while (ispunct(word[end_index])) {
            end_index--;
        }
        word = word.substr(start_index, end_index - start_index + 1);
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (m.count(word)) {
            m[word]++;
        } else {
            m[word] = 1;
        }
    }
    file.close();
    return m;
}

int main(int argc, char **argv)
{
    Stream * stream = NULL;
    PacketPtr p;
    int rc, tag=0, send_val = 100;
    char* recv_val = NULL;
    char buffer[100];
    const char* file_path_pattern = "/home/mazijun/Documents/readings/CS736/mrnet-3093918/Examples/WordCount/test-%d.txt";

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

        std::unordered_map<std::string, int> m = wordCountFile(buffer);
        std::string send_str("");

        switch(tag) {

        case PROT_SUM:
            // fprintf(stdout, "BE: count the word %s to be %d", recv_val, send_val);
            fprintf(stdout, "BE: receive the empty packet.\n");

            for (std::unordered_map<std::string, int>::iterator it = m.begin(); it != m.end(); it++) {
                send_str.append(it->first);
                send_str.append("#");
                send_str.append(std::to_string(it->second));
                send_str.append("#");
            }

            if( stream->send(tag, "%s", send_str.c_str()) == -1 ) {
                fprintf( stderr, "BE: stream::send(%%d) failure in PROT_SUM\n" );
                tag = PROT_EXIT;
                break;
            }

            fprintf(stdout, "BE: send out key value pair.\n");

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
