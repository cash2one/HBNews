:: copy命令必须要\斜杠 用/斜杠不行！
protoc gs_c_proto.proto --cpp_out=../../protobuf-msg-c++
protogen -i:gs_c_proto.proto -o:../../protobuf-msg-c#/gs_c_proto.cs
copy .\gs_c_proto.proto ..\..\protobuf-msg-c#\gs_c_proto.proto

::
protoc as_c_proto.proto --cpp_out=../../protobuf-msg-c++
protogen -i:as_c_proto.proto -o:../../protobuf-msg-c#/as_c_proto.cs
copy .\as_c_proto.proto ..\..\protobuf-msg-c#\as_c_proto.proto

::
protoc as_gs_proto.proto --cpp_out=../../protobuf-msg-c++
protoc dbs_gs_proto.proto --cpp_out=../../protobuf-msg-c++
protoc gs_gm_proto.proto --cpp_out=../../protobuf-msg-c++
protoc ps_gs_proto.proto --cpp_out=../../protobuf-msg-c++
pause  