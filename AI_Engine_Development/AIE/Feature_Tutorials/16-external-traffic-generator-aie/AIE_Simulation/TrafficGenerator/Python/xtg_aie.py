# Users need to import plio classes from the released aie_stub.
from aie_input_plio import aie_input_plio
from aie_output_plio import aie_output_plio
import linecache

def main():
    # create in/out port objects for interpolator
    in_interpolator = aie_input_plio("in_interpolator", 'int16')
    out_interpolator = aie_output_plio("out_interpolator", 'int16')

    # create in/out port objects for the classifier
    in_classifier = aie_input_plio("in_classifier",'int16')
    out_classifier = aie_output_plio("out_classifier", 'int32')

    # files in which received data will be dumped 
    recv_file_classifier = open("out_classifier_data.txt", "w")
    recv_file_interpolator = open("out_interpolator_data.txt","w")

    j = 1
    k=1

    # prepare logic to send the data 
    for i in range(0,4):
        # user logic to generate data for the classfier 
        # data values are read from file and stored as a list
        in_classifier_data = gen_data(k, k + 256, "./data/classifier_in.txt")
        k = k + 256

        # data_classifier is the sublist of in_classifier_data
        # data_classifier contains one real (16-bit) and one imag (16-bit) value 
        data_classifier = [in_classifier_data[i:i+2] for i in range(0,len(in_classifier_data),2)]

        # sending data values to the classifier
        # use send_data(data_val, tlast) API call
        # 1024 bytes are sent using send_data API call
        for data in data_classifier:
            in_classifier.send_data(data,True)
        
        # receive the data values at the o/p classifier port 
        # use receive_data() API call
	    # receive data is called 4 times
	    # Each iteration 1024 bytes are received 
        out_classifier_data = out_classifier.receive_data_with_size(1024)

        # user logic to generate data for the interpolator
        # data values are read from file and stored as a list
        in_interpolator_data = gen_data(j, j + 128, "./data/interpolator_in.txt")
        j = j + 128

        # data_interpolator is the sublist of in_interpolator_data
        # data_interpolator contains one real (16-bit) and one imag (16-bit) value 
        data_interpolator = [in_interpolator_data[i:i+2] for i in range(0,len(in_interpolator_data),2)]
        
        # sending data values to the interpolator 
        # send_data(data_val, tlast) API call
        # 512 bytes are sent using send_data API call 
        for data in data_interpolator:
            in_interpolator.send_data(data, True)

        # receive the data at the o/p interpolator port 
        # use receive_data() API call
	    # receive data is called 4 times
	    # Each iteration 1024 bytes are expected to received 
        # i.e. total 4096 bytes in 4 iterations
        out_interpolator_data = out_interpolator.receive_data_with_size(1024)

        # (optional) dump the received values in a file 
        dump_data(out_classifier_data, recv_file_classifier)

        # (optional) dump the received values in a file 
        dump_data(out_interpolator_data, recv_file_interpolator)

    # close the files
    recv_file_classifier.close()
    recv_file_interpolator.close()

# Storing required amount of data from any file into the list
# you can generate the random list as well
def gen_data(start, end, file_name):
    line_numbers = list(range(start,end))
    lines = []
    for i in line_numbers:
        x = linecache.getline(file_name, i).strip().split(' ')
        lines = lines + x
    converted_list = [int(item) for item in lines]
    return converted_list


# Logic to store the received values in a file
# which can be compared with golden file (optional)
def dump_data(data, recv_file):
    j=0
    while(j<len(data)):
        recv_file.write(str(data[j]) + "\n")
        j += 1

if __name__ == "__main__":
    main()
