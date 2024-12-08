# frozen_string_literal: true

module HipServer
  # Database adapter
  class Db
    attr_reader :db

    def initialize(name: "hip_server.db")
      db_path = File.join(__dir__, "../db_sqlite/")
      FileUtils.mkdir_p db_path
      @db = SQLite3::Database.new File.join(db_path, name)
    end

    def all_data
      @db.execute("SELECT * FROM data").map
    end

    # rubocop:disable Metrics/ParameterLists
    def create_data(
      record_type:, action:, data:, response:, uuid: SecureRandom.uuid,
      session_uuid: $session_uuid
    )
      @db.execute(
        "INSERT INTO data (uuid, session_uuid, record_type, action, data, response) VALUES (?, ?, ?, ?, ?, ?)",
        [uuid, session_uuid, record_type, action, data, response]
      )
    end
    # rubocop:enable Metrics/ParameterLists

    def drop_data
      @db.execute "DROP TABLE IF EXISTS data;"
    end

    def init
      migration_data
    end

    private

    # rubocop:disable Metrics/MethodLength
    def migration_data
      @db.execute <<-SQL
        CREATE TABLE IF NOT EXISTS data (
          uuid VARCHAR(36) NOT NULL CHECK( LENGTH(uuid) = 36 ) PRIMARY KEY,
          session_uuid VARCHAR(36) NOT NULL CHECK( LENGTH(session_uuid) = 36 ),#{" "}
          record_type TEXT NOT NULL CHECK( record_type IN ('FILE', 'DB', 'ERROR') ),
          action TEXT NOT NULL,
          data TEXT NOT NULL DEFAULT '',
          response TEXT NOT NULL DEFAULT '',
          created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
        CONSTRAINT session_uuid_unique UNIQUE (session_uuid),
        CONSTRAINT ruuid_pkey PRIMARY KEY (uuid)
      SQL
    end
    # rubocop:enable Metrics/MethodLength
  end
end
