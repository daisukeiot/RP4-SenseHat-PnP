cd cmake
cmake .. -Dskip_samples=ON -Dhsm_type_symm_key=ON -Dbuild_service_client=OFF -Dbuild_provisioning_service_client=OFF -Duse_prov_client=ON -Dhsm_type_symm_key=ON
cmake --build .