#include <iostream>
#include <string>
#include <map>
#include <tuple>

#include "usinglemon.h"

#include <lemon/smart_graph.h>
#include <lemon/lgf_writer.h>

Node getNodeByLabel(Graph* graph, 
                    NodeMap<int>* labelMap, 
                    int label);

void write2graphml(Graph* graph,
                   NodeMap<int>* labels,
                   std::map<int, EdgeMap<double>*>* edge_maps,
                   std::map<int, std::string>& edge_map_names,
                   std::map<int, int>& node_it,
                   std::string path2graphml);

int main(int argc, char* argv[])
 {
  int num_graphs { std::stoi(argv[1]) };
  // graphs
  std::map<int, Graph*> graphs;
  for (int n = 1; n <= num_graphs; n++)
    graphs.insert(std::make_pair(n, new Graph()));
  // id map
  std::map<int, NodeMap<int>*> labels;
  for (int n = 1; n <= num_graphs; n++)
    labels.insert(std::make_pair(n, new NodeMap<int>(*graphs[n])));
  // score maps
  std::map<int, EdgeMap<double>*> scores;
  for (int n = 1; n <= num_graphs; n++)
    scores.insert(std::make_pair(n, new EdgeMap<double>(*graphs[n])));
  // merged graph
  Graph merge;
  std::set<int> all_nodes;
  std::map<int, int> all_nodes_it;
  NodeMap<int> merge_labels(merge);
  std::map<int, EdgeMap<double>*> merge_scores;
  for (int n = 1; n <= num_graphs; n++)
    merge_scores.insert(std::make_pair(n, new EdgeMap<double>(merge)));
  // read graphs get all nodes  
  for (int n = 1; n <= num_graphs; n++)
   {
    std::string path2lgf { argv[n+1] };
    lemon::graphReader(*graphs[n], path2lgf).edgeMap("score", *(scores[n]))
                                            .nodeMap("label", *(labels[n]))
                                            .run();
   }
   
   int num_nodes = 0;
    
   for (int n = 1; n <= num_graphs; n++)
    {
     for (NodeIt v(*(graphs[n])); v != lemon::INVALID; ++v)
      {
       int label = (*(labels[n]))[v];
       if (all_nodes.count(label) == 0)
        {
         all_nodes.insert(label);
         num_nodes += 1;
         all_nodes_it.insert(std::make_pair(num_nodes, label));
         }
      }
    }

  std::cout << "\nMerged graph will have "
            << all_nodes.size() << " nodes.\n" << std::endl;
  
  std::map<int, Node> nodes;

  for (int n = 1; n <= all_nodes.size(); n++)
   {
    nodes[n] = merge.addNode();
    merge_labels[nodes[n]] = all_nodes_it[n];   
   }

  std::set<std::tuple<int, int>> edge_tuples;
  std::map<std::tuple<int, int>, Edge> edges;

  for (int n = 1; n <= num_graphs; n++)
   {
    std::cout << "graph " << n << std::endl;
    for (EdgeIt e(*(graphs[n])); e != lemon::INVALID; ++e)
     {
      int src_label = (*(labels[n]))[graphs[n]->u(e)];
      int trgt_label = (*(labels[n]))[graphs[n]->v(e)]; 
      std::tuple<int, int> edge_tuple (src_label, trgt_label);   
      if (edge_tuples.count(edge_tuple) == 0)
       {
        Node src = getNodeByLabel(graphs[n], labels[n], src_label);
        Node trgt = getNodeByLabel(graphs[n], labels[n], trgt_label);
        edges[edge_tuple] = merge.addEdge(src, trgt);
        edge_tuples.insert(edge_tuple);
        for (int k = 1; k <= num_graphs; k++)
         {
          if (k != n)
            (*(merge_scores[k]))[edges[edge_tuple]] = -1.0;
          else
            (*(merge_scores[k]))[edges[edge_tuple]] = (*(scores[k]))[e];        
         }
       }
      else
       (*(merge_scores[n]))[edges[edge_tuple]] = (*(scores[n]))[e];
     }
   }
  
  std::cout << "Merged graph has "
            << edges.size() << " edges."  << std::endl;  
  
  std::string outpath = argv[num_graphs + 2];
  std::map<int, std::string> edge_map_names;
  for (int n = 1; n <= num_graphs;  n++)
     edge_map_names[n] = argv[num_graphs + 2 + n];

  int ncnt = 0;
  for(NodeIt v(merge); v != lemon::INVALID; ++v)
     ncnt++;
  std::cout << ncnt << std::endl;

  int ecnt = 0;
  for(EdgeIt e(merge); e != lemon::INVALID; ++e)
     ecnt++;
  std::cout << ecnt << std::endl;

  write2graphml(&merge, &merge_labels, &merge_scores, edge_map_names, all_nodes_it, outpath);

  /*
  lemon::GraphWriter<Graph> writer(merge, outpath);
  writer.nodeMap("label", merge_labels);
  for (int n = 1; n <= num_graphs; n++)
   {
    std::string caption { argv[num_graphs + 2 + n] };
    writer.edgeMap(caption, *merge_scores[n]);
   }
  writer.run();
  */

  
  return 0;  
 }

Node getNodeByLabel(Graph* graph, NodeMap<int>* labelMap, int label)
 {
  for (NodeIt v(*graph); v != lemon::INVALID; ++v)
     if ((*labelMap)[v] == label)
       return v;
  std::cout << "No node with this label exists." << std::endl;
 }

void write2graphml(Graph* graph,
                   NodeMap<int>* labels,
                   std::map<int, EdgeMap<double>*>* edge_maps,
                   std::map<int, std::string>& edge_map_names,
                   std::map<int, int>& node_it,
                   std::string path2graphml)
 {
  std::ofstream file;
  file.open(path2graphml);
  // header
  file << "<?xml version='1.0' encoding='utf-8'?>" << "\n"
       << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" xmlns:xsi=\""
       << "http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\""
       << "http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << "\n";
  // edge attributes
  for (int n = 1; n <= edge_maps->size(); n++)
    file << "  <key attr.name=\"" << edge_map_names[n] << "\" attr.type=\"double\" for=\"edge\" id=\"d" << n-1 << "\" />" << "\n";
  // <graph>
  file << "  <graph edgedefault=\"undirected\">" << "\n";
  // nodes
  int c = 0;
  for (NodeIt v(*graph); v != lemon::INVALID; ++v)
    file << "    <node id=\"" << (*labels)[v] << "\" />" << "\n";
  //edges
  for (EdgeIt e(*graph); e != lemon::INVALID; ++e)
   {
    int ulabel = (*labels)[graph->u(e)];
    int vlabel = (*labels)[graph->v(e)];
    file << "    <edge source=\"" << ulabel << "\" target=\"" << vlabel << "\">" << "\n";
    for (int n = 1; n <= edge_maps->size(); n++)
     {
      if ((*((*edge_maps)[n]))[e] != -1)
        file << "      <data key=\"d" << n-1 << "\">" << (*((*edge_maps)[n]))[e] << "</data>" << "\n";
     }
    file << "    </edge>" << "\n";
   }
  // tail
  file << "  </graph>" << "\n";
  file << "</graphml>";
  file.close();
 }
