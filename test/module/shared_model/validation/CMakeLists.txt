addtest(composable_transaction_validator_test
        composable_transaction_validator_test.cpp
    )

target_link_libraries(composable_transaction_validator_test
        optional
        shared_model_proto_backend
        shared_model_stateless_validation
)
