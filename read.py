import pickle
from ownrecord import FileReader


def deserialize(data):
    """ deserialize bytes data    
    The deserialize method should be paired with the      
    serialize method above.    
    """     
    return pickle.loads(data)
fname = 'test.owr'
reader = FileReader(fname, check_data=True) 
print(f'Number of samples: {reader.n}')
indices = [3, 6, 0, 10]      # indices of each sample
# indices = [0,1,2]
data = reader.read(indices)  # return a list of bytes-like data
n = len(data)
for i in range(n):     
    sample = deserialize(data[i])     # do what you want
    print(sample)
reader.close()