

def makeHyperGraph(hyperedge):
    vector_things = [0 for x in range(len(hyperedge))]
    address_lookup = dict()
    for i in range(len(hyperedge)):
        node = hyperedge[i]
        vector_things[i] = node
        address_lookup[node] = i
    return (vector_things,address_lookup)


def getHyperEdge(hypergraph,odometer):
    (vector_things,address_lookup) = hypergraph
    hyperedge = [0 for x in range(len(odometer))]
    space_size = len(vector_things)
    for index in range(len(odometer)):
        node_index = odometer[index] % space_size
        hyperedge[index] = vector_things[node_index]
    return hyperedge


def getOdometer(hypergraph,hyperedge):
    (vector_things,address_lookup) = hypergraph
    odometer = [0 for x in range(len(hyperedge))]
    for index in range(len(hyperedge)):
        node = hyperedge[index]
        odometer[index] = address_lookup[node]
    return odometer



def getStartOdometer(list_of_hyperedges):
    odometer = list()
    for he in list_of_hyperedges:
        odometer.append(0)
    return odometer

def getStopOdometer(list_of_hyperedges):
    odometer = list()
    for he in list_of_hyperedges:
        odometer.append(len(he)-1)
    return odometer

def getDomainSizeList(list_of_hyperedges):
    list_of_sizes = list()
    for hyperedge in list_of_hyperedges:
        list_of_sizes.append(len(hyperedge))
    return list_of_sizes

def mul_list(list_of_numbers):
    size = 1
    for sz in list_of_numbers:
        size *= sz
    return size

def getSelection(list_of_hyperedges,odometer):
    selection = list()
    for i in range(len(list_of_hyperedges)):
        index = odometer[i]
        hyperedge = list_of_hyperedges[i]
        node = hyperedge[index]
        selection.append(node)
    return selection

def getNextOdometer(odometer,odometer_state,domain_sizes,step_size):
    control = 0
    domain_size = mul_list(domain_sizes)

    step_size = step_size % domain_size

    while control < len(odometer):
        size = domain_sizes[control]
        step = step_size % size
        step_size = step_size // size

        cur_num = odometer[control]
        dir_num = odometer_state[control]

        if step_size %2 == 1:
            cur_num = (size - 1) - cur_num
            if dir_num == 1:
                dir_num = -1
            else:
                dir_num = 1

        cur_num = cur_num + dir_num * step

        if cur_num < 0:
            cur_num +=1
            dir_num = 1
            cur_num = (size-1) - cur_num
            step_size +=1
        if cur_num >= size:
            dir_num =-1
            cur_num = (size-1) - (cur_num - size)
            step_size +=1

        odometer[control] = cur_num
        odometer_state[control] = dir_num

        if step_size == 0:
            return
        control+=1
    return

def getSampleSelection(list_of_hyperedges,number_of_samples):
    list_of_samples = list()

    domain_sizes = getDomainSizeList(list_of_hyperedges)

    total_size = mul_list(domain_sizes)

    step_size = total_size // number_of_samples

    start_odometer = getStartOdometer(list_of_hyperedges)

    odometer_state = list()
    for i in range(len(start_odometer)):
        odometer_state.append(1)

    while len(list_of_samples) < number_of_samples:
        selection = getSelection(list_of_hyperedges,start_odometer)
        list_of_samples.append(selection)
        getNextOdometer(start_odometer,odometer_state,domain_sizes,step_size)

    return list_of_samples

def test_op(list_of_hyperedges):

    start_odometer = getStartOdometer(list_of_hyperedges)

    odometer_state = list()
    for i in range(len(start_odometer)):
        odometer_state.append(1)

    print(getSelection(list_of_hyperedges,start_odometer))
    domain_sizes = getDomainSizeList(list_of_hyperedges)

    step = domain_sizes[0]*3+1
    getNextOdometer(start_odometer,odometer_state,domain_sizes,step)
    print(getSelection(list_of_hyperedges,start_odometer))

    getNextOdometer(start_odometer,odometer_state,domain_sizes,step)
    print(getSelection(list_of_hyperedges,start_odometer))

def test_code():
    list_of_simple = [ [ 1,2,3,4],
                       [1,2,3,4]]
    samples = getSampleSelection(list_of_simple,16)
    print("Sample 16 of 16")
    print(samples)
    list_of_simple = [ [ 1,2,3,4],
                       [1,2,3,4]]
    samples = getSampleSelection(list_of_simple,5)
    print("Sample 5 of 16")
    print(samples)

    list_of_medium= [  ['W','X','Y','Z'],
                            ['1','2','3','4'],
                            ['A','B','C','D']]
    samples = getSampleSelection(list_of_medium,16)
    print("Sample 16 of 64")
    print(samples)
    list_of_large_edges = [[1, 2, 3,4,5,6,7,8,9,10],
                           [1, 2, 3,4,5,6,7,8,9,10],
                           [1, 2, 3,4,5,6,7,8,9,10],
                           [1, 2, 3,4,5,6,7,8,9,10]]
    samples = getSampleSelection(list_of_large_edges,79)

    print("Sample 79 of 10000")
    print(samples)


    list_of_hyperedges_small = [[0,1,2,3],
                                [0,1,2,3]]

    samples = getSampleSelection(list_of_hyperedges_small,2)
    print("Sample 2 of 16")
    print(samples)


    list_of_hyperedges_small = [[0,1,2,3],
                                [0,1,2,3]]
    test_op(list_of_hyperedges_small)

def main():
    #test_code()

    VariableDomains = [["P0_0","P0_1","P0_2","P0_3","P1_0","P1_1","P1_2","P1_3","P2","P3","P4","P5","P6","P7"],
                       ["S0","S1","S2","S3","S4","S5","S6","S7"],
                       range(1,4001,1)]


    print("Variable Domain Sizes: " ,getDomainSizeList(VariableDomains))
    print("Total Space Size: ",mul_list(getDomainSizeList(VariableDomains)))
    samples = getSampleSelection(VariableDomains,11113)
    print("Samples:", len(samples))
    #print(samples)



if __name__ == "__main__":
    main()



