import larcv
import json
import uuid

class ConfigBuilder:

    valid_datatypes = \
        [ f"tensor{i+1}d" for i in range(4) ] + \
        [ f"sparse{i+2}d" for i in range(2) ] + \
        [ f"bbox{i+2}d" for i in range(2) ] + \
        ["particle"]
    
    valid_preprocess = [ 
        "Downsample", "DenseToSparse", "Embed", 
        "SparseToDense", "TensorFromCluster", "Threshold",
        "BBoxFromParticle"
    ]

    def __init__(self):
        self.config = larcv.QueueProcessor.default_config()

    def get_config(self): return self.config

    def set_parameter(self,value,*keys):
        """
        Sets the parameter to value, walking into the config dictionare in the order of keys
        
        :param      value:  The value
        :type       value:  { type_description }
        :param      keys:   The keys
        :type       keys:   list

        """
        dct = self.config
        for key in keys[:-1]:
               dct = dct[key]
        dct[keys[-1]] = value
        return

    def print_config(self):
        return json.dumps(self.config, indent=2)

    def validate_datatype(self, datatype : str):
        """
        Ensure datatype is viable
        
        :param      datatype:  The datatype
        :type       datatype:  str
        """

        if datatype not in self.valid_datatypes:
            raise TypeError(f"Unsupported datatype passed to ConfigBuilder: {datatype}")

    def validate_preprocess(self, preprocess : str):
        """
        Ensure preprocess is viable
        
        :param      preprocess:  The preprocess
        :type       preprocess:  str
        """

        if preprocess not in self.valid_preprocess:
            raise TypeError(f"Unsupported preprocess passed to ConfigBuilder: {preprocess}")


    def add_preprocess(self, datatype : str, producer : str, process : str, name : str = "", **kwargs):
        """
        Adds a preprocess.
        
        :param      datatype:  The datatype
        :type       datatype:  str
        :param      producer:  The producer
        :type       producer:  str
        :param      process:   The process
        :type       process:   str
        :param      name:      The name
        :type       name:      str
        :param      kwargs:    The keywords arguments
        :type       kwargs:    dictionary
        """
        
        # Validate the datatype:
        self.validate_datatype(datatype)

        # Next, validate the preprocess:
        self.validate_preprocess(process)

        # Next, we get the default config for the requested process:
        proc_config = larcv.__getattribute__(process).default_config()

        # for all of the keyword args, make sure they are in the config:
        for kwarg in kwargs:
            if kwarg not in proc_config:
                raise Exception(f"Config Parameter {kwarg} not available for"
                                f"{process}, available parameters: "
                                f"{proc_config.keys()}")

            # Override default configs:
            proc_config[kwarg] = kwargs[kwarg]

        # if the supplied name is empty, autogenerate a name:
        if name == "":
            name = f"{process}_{producer}_{datatype}_" + str(uuid.uuid4())[:8]

        proc_config["Producer"] = producer

        # Finally add this process to the configuration

        # Process Type is the 'process'
        self.config["ProcessDriver"]["ProcessType"].append(process)
        self.config["ProcessDriver"]["ProcessName"].append(name)
    
        if self.config["ProcessDriver"]["ProcessList"] is None:
            self.config["ProcessDriver"]["ProcessList"] = { name : proc_config }
        else:
            self.config["ProcessDriver"]["ProcessList"].update(
                {name : proc_config}
            )

        return


    def select_filler(self, datatype : str):
        """
        Pick the batch filler base on datatype
        
        :param      datatype:   The datatype
        :type       datatype:   str
        
        """

        if datatype == "tensor2d": return "BatchFillerTensor2D"
        elif datatype == "tensor3d": return "BatchFillerTensor3D"
        elif datatype == "sparse2d": return "BatchFillerSparseTensor2D"
        elif datatype == "sparse3d": return "BatchFillerSparseTensor3D"
        elif datatype == "bbox2d": return "BatchFillerBBox2D"
        elif datatype == "bbox3d": return "BatchFillerBBox3D"
        elif datatype == "particle": return "BatchFillerPIDLabel"
        else:
            raise Exception(f"Batch Filler not found for datatype{datatype}")


    def add_batch_filler(self, datatype : str, producer : str, name : str = "", **kwargs):
        """
        Adds a preprocess.
        
        :param      datatype:  The datatype
        :type       datatype:  str
        :param      producer:  The producer
        :type       producer:  str
        :param      name:      The name
        :type       name:      str
        :param      kwargs:    The keywords arguments
        :type       kwargs:    dictionary
        """
        
        # Similar to adding a preprocess, but the process is uniquely ID'd
        # by the datatype

        # Validate the datatype:
        self.validate_datatype(datatype)

        # Next, validate the preprocess:
        filler = self.select_filler(datatype)

        # Next, we get the default config for the requested process:
        proc_config = larcv.__getattribute__(filler).default_config()

        # for all of the keyword args, make sure they are in the config:
        for kwarg in kwargs:
            if kwarg not in proc_config:
                raise Exception(f"Config Parameter {kwarg} not available for"
                                f"{filler}, available parameters: "
                                f"{proc_config.keys()}")

            # Override default configs:
            proc_config[kwarg] = kwargs[kwarg]

        # if the supplied name is empty, autogenerate a name:
        if name == "":
            name = f"{filler}_{producer}_" + str(uuid.uuid4())[:8]
        
        proc_config["Producer"] = producer

        # Finally add this process to the configuration

        # Process Type is the 'process'
        self.config["ProcessDriver"]["ProcessType"].append(filler)
        self.config["ProcessDriver"]["ProcessName"].append(name)
    
        if self.config["ProcessDriver"]["ProcessList"] is None:
            self.config["ProcessDriver"]["ProcessList"] = { name : proc_config }
        else:
            self.config["ProcessDriver"]["ProcessList"].update(
                {name : proc_config}
            )

        return

    

