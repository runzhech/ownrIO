import pickle
from ownrecord import FileWriter


def serialize(sample):     
    """ Serialize a sample to bytes or bytearray    
    You could use anything you like to serialize the sample.    
    Here we simply use pickle.dumps().    """     
    return pickle.dumps(sample)
samples = [i for i in range(50)]  # anything you would like to store
fname = 'test.owr'
n = len(samples)  # number of samples to be written
writer = FileWriter(fname, n)  
# print("get writer:",n)
for i in range(n):     
    data = serialize(samples[i])  # data should be bytes or bytearray
    writer.write_one(bytearray(data))  
writer.close()