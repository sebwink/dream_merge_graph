
#include <lemon/smart_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>

using Graph = lemon::SmartGraph ;

using Node = lemon::SmartGraph::Node ;

template <typename T>
using NodeMap = lemon::SmartGraph::NodeMap<T> ;

using NodeIt = lemon::SmartGraph::NodeIt ;

using Edge = lemon::SmartGraph::Edge ;

template <typename T>
using EdgeMap = lemon::SmartGraph::EdgeMap<T> ;

using EdgeIt = lemon::SmartGraph::EdgeIt;
