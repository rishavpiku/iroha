hunter_upload_password(
    REPO_OWNER "soramitsu"
    REPO       "hunter-binary-cache"
    USERNAME   "warchant"
    # PASSWORD = GitHub token saved as a secure environment variable
    PASSWORD   "$ENV{GITHUB_USER_PASSWORD}" # it is actually token
)
