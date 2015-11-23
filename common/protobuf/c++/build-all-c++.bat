protoc.exe gs_c_proto.proto --cpp_out=../../protobuf-msg-c++
pause
protoc.exe as_c_proto.proto --cpp_out=../../protobuf-msg-c++
protoc.exe as_gs_proto.proto --cpp_out=../../protobuf-msg-c++
protoc.exe config.proto --cpp_out=../../protobuf-msg-c++
pause
protoc.exe hero.proto --cpp_out=../../protobuf-msg-c++


#protoc.exe tnt_deploy_goods_info.proto --cpp_out=../../protobuf-msg-c++
protoc.exe bag_item.proto --cpp_out=../../protobuf-msg-c++
protoc.exe db_user_bag.proto --cpp_out=../../protobuf-msg-c++
protoc.exe db_user_task.proto --cpp_out=../../protobuf-msg-c++
protoc.exe db_user_hero.proto --cpp_out=../../protobuf-msg-c++
protoc.exe db_user_friend.proto --cpp_out=../../protobuf-msg-c++
pause