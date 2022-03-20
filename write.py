import pickle
from pickletools import read_bytes8
from ownrecord import FileWriter

def serialize(sample):     
    """ Serialize a sample to bytes or bytearray    
    You could use anything you like to serialize the sample.    
    Here we simply use pickle.dumps().    """     
    return pickle.dumps(sample)
samples = [i for i in range(50)]  # anything you would like to store
# samples = ['123', '456', '789', '101112']
fname = 'test.owr'
n = len(samples)  # number of samples to be written
writer = FileWriter(fname, n)  
for i in range(n):     
    data = serialize(samples[i])  # data should be bytes or bytearray
    writer.write_one(data)  
writer.close()
