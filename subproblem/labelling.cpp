//  Created by Alberto Santini on 18/09/13.
//  Copyright (c) 2013 Alberto Santini. All rights reserved.
//

#include <subproblem/labelling.h>

bool Label::operator==(const Label& other) const {
    return( q_pickupable == other.q_pickupable &&
            q_deliverable == other.q_deliverable &&
            n_visited == other.n_visited &&
            fabs(cost - other.cost) < numeric_limits<float>::epsilon());
}

bool Label::operator<(const Label& other) const {
    bool strict = false;
    
    if( q_pickupable > other.q_pickupable ||
        q_deliverable > other.q_deliverable ||
        n_visited < other.n_visited ||
        cost < other.cost - numeric_limits<float>::epsilon()) {
            strict = true;
    }
    
    return( q_pickupable >= other.q_pickupable &&
            q_deliverable >= other.q_deliverable &&
            n_visited <= other.n_visited &&
            cost <= other.cost &&
            strict);
}

bool LabelExtender::operator()(const BGraph& graph, Label& new_label, const Label& label, Edge e) const {
    Node n_dest = *graph[target(e, graph)];
    
    new_label.q_pickupable = label.q_pickupable - n_dest.pu_demand();
    new_label.q_deliverable = min(label.q_deliverable - n_dest.de_demand(), label.q_pickupable - n_dest.pu_demand());
    new_label.n_visited = label.n_visited + 1;
    new_label.cost = label.cost + graph[e]->cost - label.g.dual_of(n_dest);
    
    return(label.q_pickupable >= n_dest.pu_demand() && label.q_deliverable >= n_dest.de_demand() && label.n_visited < label.g.n_port_ub() );
}